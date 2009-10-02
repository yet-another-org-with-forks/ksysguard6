#include "signalplottertest.h"
#include "../SensorDisplayLib/SignalPlotter.h"

#include <qtest_kde.h>
#include <QtTest>
#include <QtGui>

void TestSignalPlotter::init()
{
    s = new KSignalPlotter;
}
void TestSignalPlotter::cleanup()
{
    delete s;
}

void TestSignalPlotter::testReorderBeams()
{
    QCOMPARE(s->numBeams(), 0);
    QList<int> newOrder;
    s->reorderBeams(newOrder); // do nothing
    QCOMPARE(s->numBeams(), 0);

    s->addBeam(Qt::blue);
    s->addBeam(Qt::red);
    QCOMPARE(s->numBeams(), 2);
    QVERIFY(s->beamColor(0) == Qt::blue);
    QVERIFY(s->beamColor(1) == Qt::red);

    newOrder << 0 << 1; //nothing changed
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 2);
    QVERIFY(s->beamColor(0) == Qt::blue);
    QVERIFY(s->beamColor(1) == Qt::red);

    newOrder.clear();
    newOrder << 1 << 0; //reverse them
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 2);
    QVERIFY(s->beamColor(0) == Qt::red);
    QVERIFY(s->beamColor(1) == Qt::blue);

    //reverse them back again
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 2);
    QVERIFY(s->beamColor(0) == Qt::blue);
    QVERIFY(s->beamColor(1) == Qt::red);

    //switch them yet again
    s->reorderBeams(newOrder);

    //Add a third beam
    s->addBeam(Qt::green);
    QCOMPARE(s->numBeams(), 3);
    QVERIFY(s->beamColor(0) == Qt::red);
    QVERIFY(s->beamColor(1) == Qt::blue);
    QVERIFY(s->beamColor(2) == Qt::green);

    newOrder.clear();
    newOrder << 2 << 0 << 1;
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 3);
    QVERIFY(s->beamColor(0) == Qt::green);
    QVERIFY(s->beamColor(1) == Qt::red);
    QVERIFY(s->beamColor(2) == Qt::blue);
}
void TestSignalPlotter::testReorderBeamsWithData()
{
    QCOMPARE(s->numBeams(), 0);
    QList<int> newOrder;

    s->addBeam(Qt::blue);
    s->addBeam(Qt::red);
    QCOMPARE(s->numBeams(), 2);
    QCOMPARE(s->lastValue(0), 0.0);
    QCOMPARE(s->lastValue(1), 0.0);
    //Add some data
    QList<double> data;
    data << 1.0 << 2.0;
    s->addSample(data);
    QCOMPARE(s->lastValue(0), 1.0);
    QCOMPARE(s->lastValue(1), 2.0);

    newOrder << 0 << 1; //nothing changed
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 2);
    QCOMPARE(s->lastValue(0), 1.0);
    QCOMPARE(s->lastValue(1), 2.0);

    newOrder.clear();
    newOrder << 1 << 0; //reverse them
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 2);
    QCOMPARE(s->lastValue(0), 2.0);
    QCOMPARE(s->lastValue(1), 1.0);

    //reverse them back again
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 2);
    QCOMPARE(s->lastValue(0), 1.0);
    QCOMPARE(s->lastValue(1), 2.0);

    //switch them yet again
    s->reorderBeams(newOrder);

    //Add a third beam
    s->addBeam(Qt::green);
    QCOMPARE(s->numBeams(), 3);
    QCOMPARE(s->lastValue(0), 2.0);
    QCOMPARE(s->lastValue(1), 1.0);
    QCOMPARE(s->lastValue(2), 0.0);

    newOrder.clear();
    newOrder << 2 << 0 << 1;
    s->reorderBeams(newOrder);
    QCOMPARE(s->numBeams(), 3);
    QCOMPARE(s->lastValue(0), 0.0);
    QCOMPARE(s->lastValue(1), 2.0);
    QCOMPARE(s->lastValue(2), 1.0);
}
void TestSignalPlotter::testMaximumRange()
{
    QCOMPARE(s->maximumValue(), 0.0);
    QCOMPARE(s->minimumValue(), 0.0);
    QCOMPARE(s->currentMaximumRangeValue(), 0.0);
    QCOMPARE(s->currentMinimumRangeValue(), 0.0);
    QCOMPARE(s->useAutoRange(), true);

    s->addBeam(Qt::blue);
    //Nothing should have changed yet
    QCOMPARE(s->maximumValue(), 0.0);
    QCOMPARE(s->minimumValue(), 0.0);
    QCOMPARE(s->currentMaximumRangeValue(), 0.0);
    QCOMPARE(s->currentMinimumRangeValue(), 0.0);

    QList<double> data;
    data << 1.1;
    s->addSample(data);

    QCOMPARE(s->maximumValue(), 0.0);
    QCOMPARE(s->minimumValue(), 0.0);
    QCOMPARE(s->currentMaximumRangeValue(), 1.25); //It gets rounded up
    QCOMPARE(s->currentMinimumRangeValue(), 0.0);

    s->setMaximumValue(1.0);
    QCOMPARE(s->maximumValue(), 1.0);
    QCOMPARE(s->minimumValue(), 0.0);
    QCOMPARE(s->currentMaximumRangeValue(), 1.25); //Current value is still larger
    QCOMPARE(s->currentMinimumRangeValue(), 0.0);

    s->setMaximumValue(1.4);
    QCOMPARE(s->maximumValue(), 1.4);
    QCOMPARE(s->minimumValue(), 0.0);
    QCOMPARE(s->currentMaximumRangeValue(), 1.5); //given maximum range is now the larger value
    QCOMPARE(s->currentMinimumRangeValue(), 0.0);


    s->addBeam(Qt::red);
    //nothing changed by adding a beam
    QCOMPARE(s->maximumValue(), 1.4);
    QCOMPARE(s->minimumValue(), 0.0);
    QCOMPARE(s->currentMaximumRangeValue(), 1.5); //given maximum range hasn't changed
    QCOMPARE(s->currentMinimumRangeValue(), 0.0);


}
QTEST_KDEMAIN(TestSignalPlotter, GUI)

