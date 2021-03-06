/*******************************************************************************************************
 DkMetaDataWidgets.h
 Created on:	17.08.2014
 
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2014 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2014 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2014 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#pragma once

#include <QDockWidget>
#include <QTreeView>
#include <QLabel>
#include <QTextEdit>

#include "DkMetaData.h"
#include "DkImageContainer.h"
#include "DkThumbs.h"
#include "DkWidgets.h"

namespace nmc {

class DkMetaDataModel : public QAbstractTableModel {
	Q_OBJECT

public:
	DkMetaDataModel(QObject* parent = 0);
	~DkMetaDataModel();

	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;

	// return item of the model
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	//virtual Qt::ItemFlags flags(const QModelIndex& index) const;
	//virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	virtual void addMetaData(QSharedPointer<DkMetaDataT> metaData);
	void clear();

protected:
	TreeItem* rootItem;

	void createItem(const QString& key, const QString& keyName, const QString& value);

};

class DkMetaDataDock : public QDockWidget {
	Q_OBJECT

public:
	DkMetaDataDock(const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	~DkMetaDataDock();

public slots:
	void setImage(QSharedPointer<DkImageContainerT> imgC);
	void thumbLoaded(bool loaded);
	//virtual void setVisible(bool visible);

protected:
	void createLayout();
	void updateEntries();
	void writeSettings();
	void readSettings();

	void getExpandedItemNames(const QModelIndex& index, QStringList& expandedNames);
	void expandRows(const QModelIndex& index, const QStringList& expandedNames);

	QSharedPointer<DkImageContainerT> imgC;
	QTreeView* treeView;
	DkMetaDataModel* model;
	QLabel* thumbNailLabel;
	QSharedPointer<DkThumbNailT> thumb;
	QStringList expandedNames;
};

class DkMetaDataInfo : public DkWidget {
	Q_OBJECT

public:

	DkMetaDataInfo(QWidget* parent = 0);
	~DkMetaDataInfo() {};

	void draw(QPainter* painter);
	void createLabels();
	void resizeEvent(QResizeEvent *resizeW);
	//void getResolution(float &xResolution, float &yResolution);

	//DkMetaData* getMetaData() {
	//	return &metaData;
	//};

public slots:
	void setImageInfo(QSharedPointer<DkImageContainerT> imgC);
	void setRating(int rating);
	//void setResolution(int xRes, int yRes);
	void updateLabels();
	void mouseMoveEvent(QMouseEvent *event);
	void setVisible(bool visible) {

		if (visible) {
			readTags();
			createLabels();
		}

		qDebug() << "[DkMetaData] setVisible: " << visible;

		DkWidget::setVisible(visible);
	};

protected:
	void init();
	void readTags();
	void layoutLabels();
	void paintEvent(QPaintEvent *event);

	QWidget* parent;
	QPoint lastMousePos;
	QTransform worldMatrix;
	int exifHeight;
	int fontSize;
	int textMargin;
	int numLines;
	int maxCols;
	int numLabels;
	int minWidth;
	int gradientWidth;

	int yMargin;
	int xMargin;

	float currentDx;

	QVector<int> maxLenLabel;

	QVector<DkLabel *> pLabels;
	QVector<DkLabel *> pValues;
	//QSize imgSize;

	QStringList camDValues;

	QStringList descValues;

	QRect leftGradientRect;
	QLinearGradient leftGradient;
	QRect rightGradientRect;
	QLinearGradient rightGradient;

	QMap<int, int> mapIptcExif;
	QSharedPointer<DkImageContainerT> imgC;

};

class DkCommentTextEdit : public QTextEdit {
	Q_OBJECT

public:
	DkCommentTextEdit(QWidget* parent = 0);

signals:
	void focusLost();

protected:
	void focusOutEvent(QFocusEvent *focusEvent);

};

class DkCommentWidget : public DkFadeLabel {
	Q_OBJECT

public:
	DkCommentWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~DkCommentWidget() {};
	
	void setMetaData(QSharedPointer<DkMetaDataT> metaData);

public slots:
	void on_CommentLabel_textChanged();
	void on_CommentLabel_focusLost();
	void on_saveButton_clicked();
	void on_cancelButton_clicked();

signals:
	void showInfoSignal(QString msg);

protected:
	QPushButton* saveButton;
	QPushButton* cancelButton;
	QLabel* titleLabel;
	DkCommentTextEdit* commentLabel;
	QSharedPointer<DkMetaDataT> metaData;
	bool textChanged;
	bool dirty;
	QString oldText;
	QString dummyText;

	void setComment(const QString& description);
	void saveComment();
	void createLayout();
};

};
