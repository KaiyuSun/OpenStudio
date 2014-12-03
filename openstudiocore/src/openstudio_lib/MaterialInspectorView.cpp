/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "MaterialInspectorView.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialInspectorView

MaterialInspectorView::MaterialInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_roughness(nullptr),
    m_nameEdit(nullptr),
    m_thickness(nullptr),
    m_conductivity(nullptr),
    m_density(nullptr),
    m_specificHeat(nullptr),
    m_thermalAbsorptance(nullptr),
    m_solarAbsorptance(nullptr),
    m_visibleAbsorptance(nullptr),
    m_isIP(isIP)
{
  createLayout();
}

void MaterialInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  QVBoxLayout* vLayout = new QVBoxLayout();

  unsigned row = 0;

  // Name
  vLayout = new QVBoxLayout();

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,row++,0,1,3);

  // Measure Tags
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row++, 0, 1, 3);

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row++, 0);

  // Standard
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standard: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standard = new OSComboBox();
  m_standard->setEditable(true);
  m_standard->setDuplicatesEnabled(false);
  m_standard->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standard);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Standard Source
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standard Source: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardSource = new OSComboBox();
  m_standardSource->setEditable(true);
  m_standardSource->setDuplicatesEnabled(false);
  m_standardSource->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardSource);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Standards Category
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Category: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsCategory = new OSComboBox();
  m_standardsCategory->setEditable(true);
  m_standardsCategory->setDuplicatesEnabled(false);
  m_standardsCategory->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsCategory);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Standards Identifier
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Identifier: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsIdentifier = new OSComboBox();
  m_standardsIdentifier->setEditable(true);
  m_standardsIdentifier->setDuplicatesEnabled(false);
  m_standardsIdentifier->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsIdentifier);

  mainGridLayout->addLayout(vLayout, row++, 1);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row++, 0, 1, 3);

  // Roughness
  vLayout = new QVBoxLayout();

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_roughness = new OSComboBox();
  m_roughness->addItem("Very Rough");
  m_roughness->addItem("Rough");
  m_roughness->addItem("Medium Rough");
  m_roughness->addItem("Medium Smooth");
  m_roughness->addItem("Smooth");
  m_roughness->addItem("Very Smooth");
  vLayout->addWidget(m_roughness);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Thickness
  vLayout = new QVBoxLayout();

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_thickness = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_thickness);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Conductivity
  vLayout = new QVBoxLayout();

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_conductivity = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_conductivity);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Density
  vLayout = new QVBoxLayout();

  label = new QLabel("Density: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_density = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_density, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_density);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Specific Heat
  vLayout = new QVBoxLayout();

  label = new QLabel("Specific Heat: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_specificHeat = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_specificHeat, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_specificHeat);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Thermal Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_thermalAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_thermalAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_thermalAbsorptance);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Solar Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_solarAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_solarAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_solarAbsorptance);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Visible Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_visibleAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_visibleAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_visibleAbsorptance);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void MaterialInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::StandardOpaqueMaterial standardOpaqueMaterial = modelObject.cast<model::StandardOpaqueMaterial>();
  attach(standardOpaqueMaterial);
  refresh();
}

void MaterialInspectorView::onUpdate()
{
  refresh();
}

void MaterialInspectorView::attach(openstudio::model::Material & standardOpaqueMaterial)
{
  m_roughness->bind(standardOpaqueMaterial,"roughness");

  m_nameEdit->bind(standardOpaqueMaterial,"name");
  m_thickness->bind(standardOpaqueMaterial,"thickness",m_isIP);
  m_conductivity->bind(standardOpaqueMaterial,"conductivity",m_isIP);
  m_density->bind(standardOpaqueMaterial,"density",m_isIP);
  m_specificHeat->bind(standardOpaqueMaterial,"specificHeat",m_isIP);
  m_thermalAbsorptance->bind(standardOpaqueMaterial,"thermalAbsorptance",m_isIP); // Fractional, no units
  m_solarAbsorptance->bind(standardOpaqueMaterial,"solarAbsorptance",m_isIP); // Fractional, no units
  m_visibleAbsorptance->bind(standardOpaqueMaterial,"visibleAbsorptance",m_isIP); // Fractional, no units

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  m_roughness->unbind();

  m_thickness->unbind();
  m_conductivity->unbind();
  m_density->unbind();
  m_specificHeat->unbind();
  m_thermalAbsorptance->unbind();
  m_solarAbsorptance->unbind();
  m_visibleAbsorptance->unbind();
}

void MaterialInspectorView::refresh()
{
}

void MaterialInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
