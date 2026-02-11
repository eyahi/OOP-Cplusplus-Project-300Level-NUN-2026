/*
 * File: mentiontextedit.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "mentiontextedit.h"
#include <QCompleter>
#include <QStringListModel>
#include <QKeyEvent>
#include <QAbstractItemView>

MentionTextEdit::MentionTextEdit(QWidget* parent)
    : QTextEdit(parent) {
    setAcceptRichText(false);

    m_completer = new QCompleter(this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setWrapAround(false);
    connect(m_completer, QOverload<const QString&>::of(&QCompleter::activated),
            this, &MentionTextEdit::insertCompletion);
}

void MentionTextEdit::setMentions(const QStringList& names) {
    auto* model = new QStringListModel(names, m_completer);
    m_completer->setModel(model);
}

QString MentionTextEdit::wordUnderCursor() const {
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void MentionTextEdit::insertCompletion(const QString& completion) {
    if (!m_completer || m_completer->widget() != this) return;

    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, wordUnderCursor().length());
    tc.insertText(completion + " ");
    setTextCursor(tc);
}

void MentionTextEdit::keyPressEvent(QKeyEvent* e) {
    // If the completer popup is open, let it handle Enter/Return to accept a completion.
    // Otherwise the message would send before the mention gets inserted (looks like "missing words").
    if (m_completer && m_completer->popup()->isVisible()) {
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return;
            default:
                break;
        }
    }

    // Enter sends (Shift+Enter = newline)
    if ((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) && !(e->modifiers() & Qt::ShiftModifier)) {
        emit sendRequested();
        return;
    }

    QTextEdit::keyPressEvent(e);

    // Trigger completer when typing '@'
    const QString text = toPlainText();
    if (!m_completer || !m_completer->model()) return;

    if (e->text() == "@") {
        m_completer->setWidget(this);
        m_completer->complete(cursorRect());
        return;
    }

    if (m_completer->popup()->isVisible()) {
        // update suggestion based on current word
        const QString prefix = wordUnderCursor();
        m_completer->setCompletionPrefix(prefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0,0));
    }
}
