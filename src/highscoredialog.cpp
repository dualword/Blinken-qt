/* Blinken-qt (2020) http://github.com/dualword/Blinken-qt License:GNU GPL*/
/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "highscoredialog.h"

#include <QDialogButtonBox>
#include <QPainter>
#include <QPixmap>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "counter.h"

static const int margin = 15;
static const int smallMargin = 5;
static const int namesFontSize = 25;

static QSet<highScoreManager *> s_allHSM;

/* scoresWidget */

class scoresWidget : public QWidget
{
	public:
		scoresWidget(QWidget *parent, const QList< QPair<int, QString> > &scores, QSvgRenderer *renderer);
		QSize calcSize();

	protected:
		void paintEvent(QPaintEvent *) override;
	
	private:
		const QList< QPair<int, QString> > m_scores;
		QSvgRenderer *m_renderer;
};

scoresWidget::scoresWidget(QWidget *parent, const QList< QPair<int, QString> > &scores, QSvgRenderer *renderer)
 : QWidget(parent), m_scores(scores), m_renderer(renderer)
{
	setAttribute(Qt::WA_StaticContents);
}

void scoresWidget::paintEvent(QPaintEvent *)
{
	int w = width();
	int h = height();
	QFont f;
	QPixmap buf(w, h);
	QPainter p(&buf);
	QRect r;
	QColor bg = palette().window().color();
 
	// bg color
	p.fillRect(0, 0, w, h, bg);
	
	p.setPen(Qt::black);
	
	f = QFont(":/steve.ttf");
	p.setFont(f);
	f.setPointSize(15);
	p.setFont(f);
	
	p.translate(margin, margin);
	
	QList< QPair<int, QString> >::const_iterator it;
	for (it = m_scores.begin(); it != m_scores.end(); ++it)
	{
		counter::paint(p, !(*it).second.isEmpty(), (*it).first, false, QColor(), QColor(), QColor(), m_renderer);
		p.setPen(Qt::black);
		p.drawText(counter::width(false) + 2 * smallMargin, 30, (*it).second);
		p.translate(0, counter::height() + smallMargin);
	}
	
	QPainter p2(this);
	p2.drawPixmap(0, 0, buf);
}


QSize scoresWidget::calcSize()
{
	int mw, mh, lt;
	QRect r;
	QPixmap dummyPixmap(2000, 2000);
	QPainter p(&dummyPixmap);
	QFont f;
	
	f = QFont(":/steve.ttf");
	p.setFont(f);
	f.setPointSize(25);
	p.setFont(f);
	for (int i = 0; i < 3; i++)
	{
		lt = 0;
		QList< QPair<int, QString> >::const_iterator it;
		for (it = m_scores.begin(); it != m_scores.end(); ++it)
		{
			r = p.boundingRect(QRect(), Qt::AlignLeft, (*it).second);
			lt = qMax(lt, r.width());
		}
	}
	
	mw = margin + counter::width(false) + 2 * smallMargin + lt + margin;
	mh = margin * 2 + counter::height() * 5 + smallMargin * 4;
	
	QSize size(mw, mh);
	setMinimumSize(size);
	resize(size);
	
	return size;
}

/* myTabWidget */

class myTabWidget : public QTabWidget
{
	public:
		myTabWidget(QWidget *parent) : QTabWidget(parent) {}
		
		QSize tabBarSizeHint() const
		{
			return tabBar() -> sizeHint();
		}
};

/* highScoreDialog */

highScoreDialog::highScoreDialog(QWidget *parent, QSvgRenderer *renderer) : QDialog(parent)
{
	setWindowTitle(tr("window the highest scores for each level are shown", "Highscores"));

	setLayout(new QVBoxLayout(this));
	m_tw = new myTabWidget(this);
	layout()->addWidget(m_tw);
	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &highScoreDialog::close);
	layout()->addWidget(buttonBox);

	highScoreManager hsm;
	
	m_tw -> addTab(new scoresWidget(nullptr, hsm.scores(0), renderer), tr("High scores Level 1 tab title", "Level 1"));
	m_tw -> addTab(new scoresWidget(nullptr, hsm.scores(1), renderer), tr("High scores Level 2 tab title", "Level 2"));
	m_tw -> addTab(new scoresWidget(nullptr, hsm.scores(2), renderer), tr("High scores Level ? tab title", "Level ?"));
}

void highScoreDialog::showLevel(int level)
{
	QSize max, aux;
	m_tw -> setCurrentIndex(level -1);
	
	for (int i = 0; i < 3; i++)
	{
		aux = static_cast<scoresWidget*>(m_tw -> widget(i)) -> calcSize();
		max = max.expandedTo(aux);
	}
	if (max.width() < m_tw -> tabBarSizeHint().width() + 5) m_tw -> setMinimumSize(m_tw -> tabBarSizeHint().width() + 5, max.height() + m_tw -> tabBarSizeHint().height() + 5);
	
	exec();
	delete this;
}

/* highScoreManager */

highScoreManager::highScoreManager()
{
	s_allHSM << this;
	update();
}

highScoreManager::~highScoreManager()
{
	s_allHSM.remove(this);
}

bool highScoreManager::scoreGoodEnough(int level, int score)
{
	if(m_scores[--level].empty()) return true;
	QList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) ++it;
	
	return (it != itEnd);
}

void highScoreManager::addScore(int level, int score, const QString &name)
{
	if(m_scores[--level].empty()){
		m_scores[level].append(qMakePair(score, name));
	}else{
		QList< QPair<int, QString> >::iterator it, itEnd;
		it = m_scores[level].begin();
		itEnd = m_scores[level].end();
		while (it != itEnd && (*it).first >= score) ++it;

		if (it != itEnd)
		{
			m_scores[level].insert(it, qMakePair(score, name));
			if(m_scores[level].size() > 5) m_scores[level].erase(--m_scores[level].end());
		}
	}
	
	QSettings s;
	s.beginGroup(QStringLiteral("Level%1").arg(level + 1));
	int j;	
	for (int i =0;i < m_scores[level].size();i++)
	{
		s.setValue(QStringLiteral("Score%1").arg(i+1), m_scores[level].at(i).first);
		s.setValue(QStringLiteral("Name%1").arg(i+1), m_scores[level].at(i).second);
	}
	s.endGroup();

	foreach(highScoreManager *hsm, s_allHSM)
	{
		if (hsm != this)
		{
			hsm->update();
		}
	}
}

void highScoreManager::update()
{
	for (int i = 0; i < 3; ++i)
	{
		m_scores[i].clear();
	}
	QSettings s;
	for (int i = 1; i <= 3; i++)
	{
		s.beginGroup(QStringLiteral("Level%1").arg(i));
		for (int j = 1; j <= 5; j++)
		{
			int score = s.value(QStringLiteral("Score%1").arg(j), 0).toInt();
			QString name = s.value(QStringLiteral("Name%1").arg(j), "").toString();
			if(score <= 0 || name.trimmed().length() <= 0) continue;
			m_scores[i-1].append(qMakePair(score,name));
		}
		s.endGroup();
	}
}

QList< QPair<int, QString> > highScoreManager::scores(int level) const
{
	return m_scores[level];
}

int highScoreManager::score(int level, int position) const
{
	return m_scores[level][position].first;
}

QString highScoreManager::name(int level, int position) const
{
	return m_scores[level][position].second;
}
