#pragma once

#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QPushButton>

#include <NodeDataModel>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class DataLoaderModel : public NodeDataModel
{
    Q_OBJECT

public:
    DataLoaderModel();

    virtual ~DataLoaderModel() {}

public:

    QString caption() const override
    { return QString("Data Loader"); }

    QString name() const override
    { return QString("Data Loader"); }

public:

    virtual QString modelName() const
    { return QString("Data Loader"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, int) override
    { }

    QWidget * embeddedWidget() override
    { return m_pLoadDataButton; }

    bool resizable() const override
    { return false; }

private:

    void onLoadButtonClicked();

private:

    QPushButton * m_pLoadDataButton;
    QString m_sDataPath;
};
