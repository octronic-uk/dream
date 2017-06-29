/*
 * ShaderTemplateComboDelegate.cpp
 *
 * Created: 01 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "ShaderTemplateComboDelegate.h"
#include <QComboBox>
#include <QDebug>
#include <DreamCore.h>

using Dream::Constants;

ShaderTemplateComboDelegate::ShaderTemplateComboDelegate(QObject* parent)
    : QItemDelegate(parent)
{
    qDebug() << "ShaderFormatComboDelegate: Constructing Object";
}

ShaderTemplateComboDelegate::~ShaderTemplateComboDelegate
()
{
    qDebug() << "ShaderFormatComboDelegate: Destroying Object";
}

QWidget*
ShaderTemplateComboDelegate::createEditor
(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index )
const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setDuplicatesEnabled(false);
    editor->setEditable(false);
    editor->setMinimumHeight(25);
    QStringList list;
    list << QString::fromStdString(Constants::SHADER_VERTEX)
         << QString::fromStdString(Constants::SHADER_FRAGMENT);
    editor->addItems(list);
    return editor;
}

void
ShaderTemplateComboDelegate::setEditorData
(QWidget *editor, const QModelIndex &index)
const
{
    QString value = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->addItem(value);
}

void
ShaderTemplateComboDelegate::setModelData
(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value);
}

void
ShaderTemplateComboDelegate::updateEditorGeometry
(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index )
const
{
    editor->setGeometry(option.rect);
}

QSize
ShaderTemplateComboDelegate::sizeHint
( const QStyleOptionViewItem & option, const QModelIndex & index )
const
{
    return QSize(100,25);
}