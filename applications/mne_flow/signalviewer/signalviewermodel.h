#pragma once

#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <DataModelRegistry>
#include <NodeDataModel>

#include "fiffrawview.h"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class FiffRawView;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class SignalViewerModel : public NodeDataModel
{
    Q_OBJECT

public:
    SignalViewerModel();

    virtual ~SignalViewerModel() {}

public:

    QString caption() const override
    { return QString("Signal Viewer"); }

    QString name() const override
    { return QString("Signal Viewer"); }

public:

    virtual QString modelName() const
    { return QString("Signal Viewer"); }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex port) override;

    QWidget * embeddedWidget() override
    { return m_pFiffRawView; }

    bool resizable() const override
    { return true; }

private:

    FiffRawView * m_pFiffRawView;
};
