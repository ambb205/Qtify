#include "include/spotify_delegate.h"
#include "include/spotify_result.h"
#include <iostream>
void Spotify_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if (index.data().canConvert<Spotify_Result *>())
    {
        Spotify_Result *result = qvariant_cast<Spotify_Result *>(index.data());
        result->set_rect(option.rect);
        if (result->downloaded_)
        {
            if (option.state & QStyle::State_Selected)
                painter->fillRect(option.rect, option.palette.highlight());
            result->paint(painter, option.rect, option.palette);
        }
        else
        {
            if (option.state & QStyle::State_Selected)
                painter->fillRect(option.rect, option.palette.highlight());
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QWidget *Spotify_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const

{
    return QStyledItemDelegate::createEditor(parent, option, index);
}
void Spotify_Delegate::commitAndCloseEditor()
{
    //    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    //    emit commitData(editor);
    //    emit closeEditor(editor);
}

void Spotify_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QStyledItemDelegate::setEditorData(editor, index);
}

void Spotify_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    QStyledItemDelegate::setModelData(editor, model, index);
}
QSize Spotify_Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().canConvert<Spotify_Result *>())
    {
        Spotify_Result *result = qvariant_cast<Spotify_Result *>(index.data());
        return result->sizeHint();
    }
    return QStyledItemDelegate::sizeHint(option, index);
}
