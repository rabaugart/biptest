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
#include <QLineEdit>

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
    QLabel* labelLabel;
    QLineEdit* signalTransport;

    QStringListModel fieldModel;

    std::string currentSegment;
    std::string currentField;

    GAdapterFactory::adapter_ptr current_adapter;

    boost::optional<boost::signals2::connection> connection;

    void operator()(GAdapterFactory::signal_frame_t const& sig);

    void segmentChanged(QString const&);
    void fieldChanged(QString const&);

    struct Visitor: public boost::static_visitor<void> {

        Visitor(QLineEdit* l) :
            signalTransport(l) {
        }
        QLineEdit* signalTransport;
        template<typename T>
        void operator()(T) {
        }
        ;
        void operator()(double d) {
            signalTransport->setText(QString::number(d));
        }
    };

    Visitor visit;
};

AdapterGroup::AdapterGroup(QWidget* parent, GAdapterFactory& f) :
        QObject(parent), factory(f), segmentComboBox(new QComboBox(parent)), fieldComboBox(
                new QComboBox(parent)), valueLabel(new QLabel(parent)), labelLabel(new QLabel(parent)), signalTransport(
                new QLineEdit(parent)), visit(signalTransport) {
    signalTransport->hide();
    segmentComboBox->setModel(&factory.segmentListModel);
    segmentComboBox->setCurrentIndex( -1 );
    fieldComboBox->setModel(&fieldModel);
    QObject::connect(segmentComboBox,
            static_cast<void (QComboBox::*)(
                    const QString &)>(&QComboBox::currentIndexChanged), this, &AdapterGroup::segmentChanged);
    QObject::connect(fieldComboBox,
            static_cast<void (QComboBox::*)(
                    const QString &)>(&QComboBox::currentIndexChanged), this, &AdapterGroup::fieldChanged);
    QObject::connect(signalTransport, &QLineEdit::textChanged, valueLabel,
            &QLabel::setText, Qt::QueuedConnection);
    valueLabel->setText("");

}

void AdapterGroup::segmentChanged(QString const& index) {
    std::cout << "New segment index " << index.toStdString() << std::endl;
    if (index != "") {
        currentSegment = index.toStdString();
        std::vector<GAdapterFactory::descriptor_t> const descrs =
                factory.fieldDescriptors(currentSegment);
        QStringList l;
        for (auto const& i : descrs) {
            l << QString::fromStdString(i.key);
        }
        if (connection) {
            connection->disconnect();
            connection=boost::none;
        }
        labelLabel->setText("");
        valueLabel->setText("");
        fieldModel.setStringList(l);
    }
}

void AdapterGroup::fieldChanged(QString const& index) {
    std::cout << "New field index " << index.toStdString() << std::endl;
    if (index != "") {
        currentField = index.toStdString();
        if (connection) {
            connection->disconnect();
            connection = boost::none;
        }
        current_adapter = factory.makeAdapter(currentSegment, currentField);
        labelLabel->setText( QString::fromStdString(current_adapter->descr.label) );
        connection = current_adapter->sigValue.connect(std::ref(*this));
    }
}

void AdapterGroup::operator()(GAdapterFactory::signal_frame_t const& sig) {
    if (sig.valid) {
        boost::apply_visitor(visit, sig.value);
    } else {
        signalTransport->setText("###");
    }
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
    QGridLayout* layout = new QGridLayout(central);
    setCentralWidget(central);
    central->setLayout(layout);

    QLabel* title = new QLabel(this);
    title->setText("Monitor");
    title->setAlignment( Qt::AlignHCenter );
    layout->addWidget(title,0,0,1,-1);

    QLabel* segmentHead = new QLabel(this);
    segmentHead->setText("Segment");
    layout->addWidget(segmentHead,1,0);

    QLabel* fieldHead = new QLabel(this);
    fieldHead->setText("Field");
    layout->addWidget(fieldHead,1,1);

    QLabel* labelHead = new QLabel(this);
    labelHead->setText("Label");
    layout->addWidget(labelHead,1,2);

    QLabel* valueHead = new QLabel(this);
    valueHead->setText("Value");
    layout->addWidget(valueHead,1,3);

    for (size_t i = 0; i < 4; i++) {
        int const row = i+2;
        AdapterGroup* ad = new AdapterGroup(this, factory);
        layout->addWidget(ad->segmentComboBox, row, 0);
        layout->addWidget(ad->fieldComboBox, row, 1);
        layout->addWidget(ad->labelLabel, row, 2);
        layout->addWidget(ad->valueLabel, row, 3);
    }
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    app.setApplicationName("GMonitor");

    MainWindow main(&app);

    main.show();

    return app.exec();
}

