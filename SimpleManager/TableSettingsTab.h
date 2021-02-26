#pragma once

#include <QWidget>
#include "ui_TableSettingsTab.h"

class TableSettingsTab : public QWidget
{
	Q_OBJECT

public:
	TableSettingsTab(QWidget *parent = Q_NULLPTR);
	~TableSettingsTab();

private:
	Ui::TableSettingsTab ui;
};
