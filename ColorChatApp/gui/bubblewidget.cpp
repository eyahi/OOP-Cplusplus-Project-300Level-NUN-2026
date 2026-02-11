/*
 * File: bubblewidget.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "bubblewidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QFontMetrics>
#include <QSizePolicy>

static QString fmtTime(const QDateTime& dt) {
    return dt.toLocalTime().toString("HH:mm");
}

// Helps very long "unbroken" tokens (URLs, hashes...) wrap inside a QLabel.
// QLabel wordWrap wraps mostly at spaces; for long tokens we insert zero-width spaces.
static QString softWrapLongTokens(const QString& in) {
    QString out;
    out.reserve(in.size() + in.size() / 6);

    const QChar ZWS(0x200B); // zero-width space
    int run = 0;

    auto flushRun = [&](){ run = 0; };

    for (int i = 0; i < in.size(); ++i) {
        const QChar ch = in.at(i);
        const bool isBreak = ch.isSpace() || ch == '-' || ch == '/' || ch == '\\';
        if (isBreak) {
            flushRun();
            out.append(ch);
            continue;
        }

        out.append(ch);
        run++;
        // Insert a soft break every 18 chars in a long run.
        if (run >= 18) {
            out.append(ZWS);
            run = 0;
        }
    }

    return out;
}

BubbleWidget::BubbleWidget(const QString& senderName,
                           const QString& text,
                           const QDateTime& timestamp,
                           bool mine,
                           const QString& replyPreview,
                           bool lightTheme,
                           QWidget* parent)
    : QFrame(parent),
      m_previewText(text),
      m_senderName(senderName),
      m_bodyRaw(text),
      m_mine(mine) {

    setObjectName("BubbleWidget");
    setFrameShape(QFrame::NoFrame);

    auto* outer = new QHBoxLayout(this);
    outer->setContentsMargins(6, 2, 6, 2);

    m_bubble = new QFrame(this);
    m_bubble->setObjectName("Bubble");
    m_bubble->setProperty("mine", mine);
    m_bubble->setMaximumWidth(m_maxBubbleWidth);
    m_bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    auto* v = new QVBoxLayout(m_bubble);
    v->setSpacing(6);
    v->setContentsMargins(14, 12, 14, 10);

    m_sender = new QLabel(senderName, m_bubble);
    m_sender->setObjectName("Sender");
    m_sender->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_sender->setVisible(!mine);

    m_reply = new QLabel(m_bubble);
    m_reply->setObjectName("ReplyPreview");
    m_reply->setWordWrap(true);
    m_reply->setVisible(!replyPreview.isEmpty());
    m_reply->setText(softWrapLongTokens(replyPreview));

    m_body = new QLabel(softWrapLongTokens(text), m_bubble);
    m_body->setObjectName("Body");
    m_body->setWordWrap(true);
    m_body->setTextInteractionFlags(Qt::TextSelectableByMouse);

    m_meta = new QLabel(fmtTime(timestamp) + (mine ? "  ✓" : ""), m_bubble);
    m_meta->setObjectName("Meta");

    v->addWidget(m_sender);
    v->addWidget(m_reply);
    v->addWidget(m_body);
    v->addWidget(m_meta, 0, Qt::AlignRight);

    // Proper left/right alignment while still allowing the bubble to expand.
    if (mine) {
        outer->addStretch(1);
        outer->addWidget(m_bubble, 0, Qt::AlignRight);
    } else {
        outer->addWidget(m_bubble, 0, Qt::AlignLeft);
        outer->addStretch(1);
    }

    recomputeWidth();

    // Bubble styles: use slightly different contrasts on Light theme so bubbles remain readable.
    if (lightTheme) {
        m_bubble->setStyleSheet(R"QSS(
            QFrame#Bubble[mine="true"]{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 rgba(0,255,255,0.26), stop:0.5 rgba(255,0,255,0.18), stop:1 rgba(0,180,255,0.22));
                border: 1px solid rgba(0,180,255,0.25);
                border-radius: 18px;
            }
            QFrame#Bubble[mine="false"]{
                background: rgba(17,24,39,0.04);
                border: 1px solid rgba(17,24,39,0.10);
                border-radius: 18px;
            }
            QLabel#Sender { font-weight: 700; opacity: 0.90; }
            QLabel#ReplyPreview {
                background: rgba(17,24,39,0.03);
                border: 1px solid rgba(17,24,39,0.10);
                border-radius: 12px;
                padding: 8px 10px;
                opacity: 0.95;
            }
            QLabel#Body { font-size: 13.5px; }
            QLabel#Meta { font-size: 11px; opacity: 0.70; }
        )QSS");
    } else {
        m_bubble->setStyleSheet(R"QSS(
            QFrame#Bubble[mine="true"]{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 rgba(0,255,255,0.18), stop:0.5 rgba(255,0,255,0.14), stop:1 rgba(0,180,255,0.16));
                border: 1px solid rgba(0,255,255,0.20);
                border-radius: 18px;
            }
            QFrame#Bubble[mine="false"]{
                background: rgba(255,255,255,0.06);
                border: 1px solid rgba(255,255,255,0.12);
                border-radius: 18px;
            }
            QLabel#Sender { font-weight: 700; opacity: 0.9; }
            QLabel#ReplyPreview {
                background: rgba(0,0,0,0.22);
                border: 1px solid rgba(255,255,255,0.10);
                border-radius: 12px;
                padding: 8px 10px;
                opacity: 0.95;
            }
            QLabel#Body { font-size: 13.5px; }
            QLabel#Meta { font-size: 11px; opacity: 0.75; }
        )QSS");
    }

    // Pop-in animation
    auto* fx = new QGraphicsOpacityEffect(m_bubble);
    m_bubble->setGraphicsEffect(fx);
    fx->setOpacity(0.0);

    auto* anim = new QPropertyAnimation(fx, "opacity", this);
    anim->setDuration(180);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BubbleWidget::setMaxBubbleWidth(int px) {
    if (px < 260) px = 260;
    if (px == m_maxBubbleWidth) return;
    m_maxBubbleWidth = px;
    if (m_bubble) {
        m_bubble->setMaximumWidth(px);
    }
    recomputeWidth();
}

void BubbleWidget::recomputeWidth() {
    if (!m_bubble || !m_body || !m_meta) return;

    const int kMax = m_maxBubbleWidth;

    const QFontMetrics bodyFm(m_body->font());
    const int bodyW = bodyFm.horizontalAdvance(m_bodyRaw);
    const int metaW = QFontMetrics(m_meta->font()).horizontalAdvance(m_meta->text());
    const int senderW = (!m_mine && m_sender) ? QFontMetrics(m_sender->font()).horizontalAdvance(m_senderName) : 0;

    int desiredW = qMax(bodyW, qMax(metaW, senderW));

    // Account for inner padding (14 + 14) and a bit of safety.
    desiredW += 14 + 14 + 18;

    if (desiredW < 220) desiredW = 220;
    if (desiredW > kMax) desiredW = kMax;

    m_bubble->setMinimumWidth(desiredW);
}

