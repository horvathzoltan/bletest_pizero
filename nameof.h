#ifndef NAMEOF_H
#define NAMEOF_H

#include <QRegularExpression>
#include <QString>

#define nameof(x) _nameof<0>(#x, sizeof(x))
#define nameof_fn(x) _nameof_fn<0>(#x, (void*)x)

QString NameOf(const QString& x){
    static const QRegularExpression refn(QStringLiteral(R"((?:::|->|\.)?([\w]+)\(\)$)"));
    static const QRegularExpression re(QStringLiteral(R"((?:->|\.|::)?([_a-zA-Z]\w*)$)"));
    QRegularExpressionMatch m;

    if(x.endsWith(QStringLiteral("()"))) m = refn.match(x);
    else m = re.match(x);

    if (!m.hasMatch()) return QStringLiteral("A bad expression in nameof(%1)").arg(x);

    auto name = m.captured(m.lastCapturedIndex());
    return name;
}

template<int a>
QString _nameof_fn(const char* y, void*)
{
    return NameOf(QString(y));
}

template<int a>
QString _nameof(const char* y, std::size_t)
{
    return NameOf(QString(y));
}

#endif // NAMEOF_H
