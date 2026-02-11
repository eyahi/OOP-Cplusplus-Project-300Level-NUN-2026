/*
 * File: mentiontextedit.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#pragma once
#include <QTextEdit>
#include <QStringList>

class QCompleter;

class MentionTextEdit : public QTextEdit {
    Q_OBJECT
public:
    explicit MentionTextEdit(QWidget* parent = nullptr);

    void setMentions(const QStringList& names);

signals:
    void sendRequested();

protected:
    void keyPressEvent(QKeyEvent* e) override;

private slots:
    void insertCompletion(const QString& completion);

private:
    QString wordUnderCursor() const;

    QCompleter* m_completer{nullptr};
};
