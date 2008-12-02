/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** 
** Non-Open Source Usage  
** 
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.  
** 
** GNU General Public License Usage 
** 
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception version
** 1.2, included in the file GPL_EXCEPTION.txt in this package.  
**
***************************************************************************/

#ifndef PROITEMINFO_H
#define PROITEMINFO_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtXml/QDomElement>

#include "proitems.h"

namespace Qt4ProjectManager {
namespace Internal {

class ProItemInfo
{
public:
    enum ProItemInfoKind {
        Scope,
        Value,
        Variable
    };

    ProItemInfo(ProItemInfoKind kind);

    ProItemInfoKind kind() const;
    void setId(const QString &id);
    void setName(const QString &name);
    void setDescription(const QString &desc);

    QString id() const;
    QString name() const;
    QString description() const;

private:
    QString m_id;
    QString m_name;
    QString m_description;
    ProItemInfoKind m_kind;
};

class ProScopeInfo : public ProItemInfo
{
public:
    ProScopeInfo();
};

class ProValueInfo : public ProItemInfo
{
public:
    ProValueInfo();
};

class ProVariableInfo : public ProItemInfo
{
public:
    ProVariableInfo();
    ~ProVariableInfo();

    void addValue(ProValueInfo *value);
    void setMultiple(bool multiple);
    void setDefaultOperator(ProVariable::VariableOperator op);

    ProValueInfo *value(const QString &id) const;

    QList<ProValueInfo *> values() const;
    bool multiple() const;
    ProVariable::VariableOperator defaultOperator() const;

private:
    ProVariable::VariableOperator m_operator;
    bool m_multiple;
    QMap<QString, ProValueInfo *> m_values;
};

class ProItemInfoManager : public QObject {
    Q_OBJECT

public:
    ProItemInfoManager(QObject *parent);
    ~ProItemInfoManager();

    ProVariableInfo *variable(const QString &id) const;
    ProScopeInfo *scope(const QString &id) const;

    QList<ProScopeInfo *> scopes() const;
    QList<ProVariableInfo *> variables() const;

private:
    bool load(const QString &filename);
    void addVariable(ProVariableInfo *variable);
    void addScope(ProScopeInfo *scope);
    void readItem(ProItemInfo *item, const QDomElement &data);
    void readScope(const QDomElement &data);
    void readVariable(const QDomElement &data);

    QMap<QString, ProScopeInfo *> m_scopes;
    QMap<QString, ProVariableInfo *> m_variables;
};

} // namespace Internal
} // namespace Qt4ProjectManager

#endif // PROITEMINFO_H
