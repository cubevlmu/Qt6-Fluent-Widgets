#include "Router.hpp"

namespace fluent {

    RouteItem::RouteItem() {}
    

    RouteItem::RouteItem(QStackedWidget* stacked, const QString& routeKey)
        : stacked(stacked), routeKey(routeKey) {}


    bool RouteItem::operator==(const RouteItem& other) const {
        return other.stacked == stacked && other.routeKey == routeKey;
    }


    StackedHistory::StackedHistory(QStackedWidget* stacked)
        : stacked(stacked)
    {
        history.append(defaultRouteKey);
    }


    int StackedHistory::size() const {
        return history.size();
    }


    bool StackedHistory::isEmpty() const {
        return size() <= 1;
    }


    bool StackedHistory::push(const QString& routeKey) {
        if (history.last() == routeKey) {
            return false;
        }

        history.append(routeKey);
        return true;
    }


    void StackedHistory::pop() {
        if (isEmpty()) {
            return;
        }

        history.pop_back();
        goToTop();
    }


    void StackedHistory::remove(const QString& routeKey) {
        if (!history.contains(routeKey)) {
            return;
        }

        history.erase(std::remove(history.begin() + 1, history.end(), routeKey), history.end());
        history.erase(std::unique(history.begin(), history.end()), history.end());
        goToTop();
    }


    QString StackedHistory::top() const {
        return history.last();
    }


    void StackedHistory::setDefaultRouteKey(const QString& routeKey) {
        defaultRouteKey = routeKey;
        history[0] = routeKey;
    }


    void StackedHistory::goToTop() {
        QWidget* w = stacked->findChild<QWidget*>(top());
        if (w) {
            stacked->setCurrentWidget(w);
        }
    }



    Router::Router(QObject* parent)
        : QObject(parent)
    {}


    void Router::setDefaultRouteKey(
        QStackedWidget* stacked,
        const QString& routeKey
    ) {
        if (!stackHistories.contains(stacked)) {
            stackHistories.insert(stacked, new StackedHistory(stacked));
        }

        stackHistories[stacked]->setDefaultRouteKey(routeKey);
    }


    void Router::push(QStackedWidget* stacked, const QString& routeKey) {
        RouteItem item(stacked, routeKey);

        if (!stackHistories.contains(stacked)) {
            stackHistories.insert(stacked, new StackedHistory(stacked));
        }

        bool success = stackHistories[stacked]->push(routeKey);
        if (success) {
            history.push(item);
        }

        emit emptyChanged(history.isEmpty());
    }


    void Router::pop() {
        if (history.isEmpty()) {
            return;
        }

        RouteItem item = history.pop();
        emit emptyChanged(history.isEmpty());
        stackHistories[item.stacked]->pop();
    }


    void Router::remove(const QString& routeKey) {
        history.erase(std::remove_if(history.begin(), history.end(),
            [&](const RouteItem& item) { return item.routeKey == routeKey; }),
            history.end());

        history.erase(std::unique(history.begin(), history.end(),
            [&](const RouteItem& a, const RouteItem& b) { return a.routeKey == b.routeKey; }),
            history.end());

        emit emptyChanged(history.isEmpty());

        for (auto* stacked : stackHistories.keys()) {
            QWidget* w = stacked->findChild<QWidget*>(routeKey);
            if (w) {
                stackHistories[stacked]->remove(routeKey);
            }
        }
    }

} // namespace fluent