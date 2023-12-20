#include "myparserrunnable.h"

#include <qfile.h>

MyParserRunnable::MyParserRunnable(const QString &sPath,
                                   const QString &sIncludeText,
                                   const QString &sExcludeText,
                                   int nVersion,
                                   bool bUseOldStyleStream,
                                   bool bIsCaseSensetiveInclude,
                                   bool bSearchFullPhraseInclude,
                                   bool bIsCaseSensetiveExclude,
                                   bool bSearchFullPhraseExclude,
                                   QSharedPointer<bool> &bForceStop)
    : QObject()
    , QRunnable()
    , m_sPath(sPath)
    , m_bIsCaseSensetiveInclude(bIsCaseSensetiveInclude)
    , m_bSearchFullPhraseInclude(bSearchFullPhraseInclude)
    , m_bIsCaseSensetiveExclude(bIsCaseSensetiveExclude)
    , m_bSearchFullPhraseExclude(bSearchFullPhraseExclude)
    , m_nVersion(nVersion)
    , m_bUseOldStyleStream(bUseOldStyleStream)
    , m_bForceStop(bForceStop)
{
    m_vecIncludeText = sIncludeText.split('\n');
    if (m_bSearchFullPhraseInclude) {
        for (auto &str : m_vecIncludeText) {
            if (str.isEmpty())
                continue;
            QRegularExpression obj;
            obj.setPattern("\\b" + str + "\\b");
            if (!bIsCaseSensetiveInclude)
                obj.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
            m_vecRegExpInclude.emplaceBack(obj);
        }
    }

    if (!sExcludeText.isEmpty()) {
        m_vecExcludeText = sExcludeText.split('\n');
        if (m_bSearchFullPhraseExclude) {
            for (auto &str : m_vecExcludeText) {
                if (str.isEmpty())
                    continue;
                QRegularExpression obj;
                obj.setPattern("\\b" + str + "\\b");
                if (!bIsCaseSensetiveExclude)
                    obj.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
                m_vecRegExpExclude.emplaceBack(obj);
            }
        }
    }
}

bool MyParserRunnable::CheckIsTextSuitable(const QString &sText)
{
    if (sText.isEmpty())
        return false;

    bool bFind{false};
    if (m_bSearchFullPhraseInclude) {
        for (auto &obj : m_vecRegExpInclude) {
            if (!sText.contains(obj))
                continue;

            bFind = true;
            break;
        }
    } else {
        for (auto &str : m_vecIncludeText) {
            if (str.isEmpty())
                continue;

            if (!sText.contains(str,
                                m_bIsCaseSensetiveInclude ? Qt::CaseSensitivity::CaseSensitive
                                                          : Qt::CaseSensitivity::CaseInsensitive))
                continue;

            bFind = true;
            break;
        }
    }

    if (!bFind)
        return false;

    bFind = false;
    if (m_bSearchFullPhraseExclude) {
        for (auto &obj : m_vecRegExpExclude) {
            if (!sText.contains(obj))
                continue;

            bFind = true;
            break;
        }
    } else {
        for (auto &str : m_vecExcludeText) {
            if (str.isEmpty())
                continue;

            if (!sText.contains(str,
                                m_bIsCaseSensetiveExclude ? Qt::CaseSensitivity::CaseSensitive
                                                          : Qt::CaseSensitivity::CaseInsensitive))
                continue;

            bFind = true;
            break;
        }
    }

    if (bFind)
        return false;

    return true;
}

void MyParserRunnable::UseQByteArray()
{
    QFile file(m_sPath);
    auto pResult = new QTreeWidgetItem(QStringList{m_sPath, "", ""});

    if (!file.open(QIODevice::ReadOnly)) {
        emit SignalSendResult(pResult, m_nVersion);
        return;
    }

    int nLine{0};

    QByteArray data = file.readAll();
    auto text{QString::fromLocal8Bit(data)};
    data.clear();
    auto listSplited = text.split('\n');
    text.clear();

    for (const auto &sLine : listSplited) {
        if (*m_bForceStop)
            break;

        ++nLine;
        auto sLineText = sLine.trimmed();

        if (!CheckIsTextSuitable(sLineText))
            continue;

        pResult->addChild(new QTreeWidgetItem(QStringList{"", QString::number(nLine), sLineText}));
    }
    file.close();

    emit SignalSendResult(pResult, m_nVersion);
}

void MyParserRunnable::UseQStream()
{
    QFile file(m_sPath);
    auto pResult = new QTreeWidgetItem(QStringList{m_sPath, "", ""});

    if (!file.open(QIODevice::ReadOnly)) {
        emit SignalSendResult(pResult, m_nVersion);
        return;
    }

    int nLine{0};
    QTextStream in(&file);

    while (!in.atEnd()) {
        if (*m_bForceStop)
            break;

        ++nLine;
        auto sLineText = in.readLine().trimmed();

        if (!CheckIsTextSuitable(sLineText))
            continue;

        pResult->addChild(new QTreeWidgetItem(QStringList{"", QString::number(nLine), sLineText}));
    }
    file.close();

    emit SignalSendResult(pResult, m_nVersion);
}

void MyParserRunnable::run()
{
    if (m_bUseOldStyleStream)
        UseQByteArray();
    else
        UseQStream();
}
