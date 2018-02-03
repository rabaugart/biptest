/*
 * gmonitor.cpp
 *
 *  Created on: 03.02.2018
 *      Author: netz
 */

#include <iostream>

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QStringListModel>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

#include "sscshm/SscFieldAdapterFactory.h"

using ran::ssc::SscFieldAdapterFactory;

class GAdapterFactory: public SscFieldAdapterFactory {
public:

    typedef adapter_t::FieldDescriptor descriptor_t;

    GAdapterFactory() :
            segmentListModel([this]() {
                QStringList ret;
                std::vector<std::string> segNames = segmentNames();
                for ( auto const& i : segNames ) {
                    ret << QString::fromStdString(i);
                }
                return ret;
            }()) {
    }

    QStringListModel segmentListModel;
};

class AdapterGroup: public QObject {
public:
    explicit AdapterGroup(QWidget* parent, GAdapterFactory& f);

    GAdapterFactory& factory;
    QComboBox* segmentComboBox;
    QComboBox* fieldComboBox;
    QLabel* valueLabel;

    QStringListModel fieldModel;

    void segmentChanged(QString const&);
};

AdapterGroup::AdapterGroup(QWidget* parent, GAdapterFactory& f) :
        QObject(parent), factory(f), segmentComboBox(new QComboBox(parent)), fieldComboBox(
                new QComboBox(parent)), valueLabel(new QLabel(parent)) {
    segmentComboBox->setModel(&factory.segmentListModel);
    fieldComboBox->setModel(&fieldModel);
    QObject::connect(segmentComboBox,
            static_cast<void (QComboBox::*)(
                    const QString &)>(&QComboBox::currentIndexChanged), this, &AdapterGroup::segmentChanged);
    valueLabel->setText("xxx");

}

void AdapterGroup::segmentChanged(QString const& index) {
    std::cout << "New index " << index.toStdString() << std::endl;
    std::vector<GAdapterFactory::descriptor_t> const descrs = factory.fieldDescriptors(index.toStdString());
    QStringList l;
    for ( auto const& i : descrs ) {
        l << QString::fromStdString(i.key);
    }
    fieldModel.setStringList(l);
}

class MainWindow: public QMainWindow {
public:

    MainWindow(QObject* parent);

    ~MainWindow() {
    }

protected:
    GAdapterFactory factory;
};

MainWindow::MainWindow(QObject* parent) {

    QWidget* central = new QWidget(this);
    QGridLayout* layout = new QGridLayout(this);
    central->setLayout(layout);
    setCentralWidget(central);
    for (size_t i = 0; i < 3; i++) {
        AdapterGroup* ad = new AdapterGroup(this, factory);
        layout->addWidget(ad->segmentComboBox, i, 0);
        layout->addWidget(ad->fieldComboBox, i, 1);
        layout->addWidget(ad->valueLabel, i, 2);
    }
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    MainWindow main(&app);

    main.show();

    return app.exec();
}

