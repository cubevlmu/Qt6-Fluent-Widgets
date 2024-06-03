#pragma once

#include <QObject>
#include <QStackedWidget>
#include <QWidget>
#include <QString>
#include <QVector>
#include <QMap>
#include <QStack>
#include <algorithm>

namespace fluent {

    class RouteItem {
    public:
        RouteItem();
        RouteItem(QStackedWidget* stacked, const QString& routeKey);

        bool operator==(const RouteItem& other) const;

        QStackedWidget* stacked;
        QString routeKey;
    };


    class StackedHistory {
    public:
        StackedHistory(QStackedWidget* stacked);

        int size() const;
        bool isEmpty() const;

        bool push(const QString& routeKey);
        void pop();

        void remove(const QString& routeKey);

        QString top() const;

        void setDefaultRouteKey(const QString& routeKey);
        void goToTop();

    private:
        QStackedWidget* stacked;
        QString defaultRouteKey;
        QVector<QString> history;
    };


    class Router : public QObject {
        Q_OBJECT

    public:

        Router(QObject* parent = nullptr);

        void setDefaultRouteKey(QStackedWidget* stacked, const QString& routeKey);

        void push(QStackedWidget* stacked, const QString& routeKey);
        void pop();

        void remove(const QString& routeKey);

    signals:
        void emptyChanged(bool isEmpty);

    private:
        QStack<RouteItem> history;
        QMap<QStackedWidget*, StackedHistory*> stackHistories;
    };


    Q_GLOBAL_STATIC(Router, router);
}