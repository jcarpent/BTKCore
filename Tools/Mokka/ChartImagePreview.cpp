/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ChartImagePreview.h"

#include <QKeyEvent>
#include <QMouseEvent>

#include <btkVTKChartTimeSeries.h>
#include <btkVTKChartLegend.h>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkAxis.h>
#include <vtkPlot.h>
#include <vtkPlotLine.h>
#include <vtkContextActor.h>
#include <vtkContextScene.h>
#include <vtkUnsignedCharArray.h>
#include <vtkTextProperty.h>

ChartImagePreview::ChartImagePreview(QWidget* parent)
: QVTKWidget(parent)
{
  // Member
  this->mp_Renderer = vtkRenderer::New();
  this->mp_Renderer->SetBackground(1.0,1.0,1.0);
  
  // No need to send mouse events to VTK when a mouse button isn't down
  this->setMouseTracking(false);
}

ChartImagePreview::~ChartImagePreview()
{
  this->mp_Chart->Delete();
  this->mp_Renderer->Delete();
};

void ChartImagePreview::initialize()
{
  vtkRenderWindow* renwin = vtkRenderWindow::New();
  // renwin->OffScreenRenderingOn();
  renwin->DoubleBufferOff();
  // renwin->SwapBuffersOff();
  renwin->AddRenderer(this->mp_Renderer);
  this->SetRenderWindow(renwin);
  renwin->Delete();
  
  this->mp_Chart = btk::VTKChartTimeSeries::New();
  vtkChartLegend* legend = btk::VTKChartLegend::New();
  this->mp_Chart->SetLegend(legend);
  this->mp_Chart->SetShowLegend(true);
  
  vtkContextScene* scene = vtkContextScene::New();
  vtkContextActor* actor = vtkContextActor::New();
  scene->AddItem(this->mp_Chart);
  actor->SetScene(scene);
  this->mp_Renderer->AddActor(actor);
  scene->SetRenderer(this->mp_Renderer);
  
  legend->Delete();
  actor->Delete();
  scene->Delete();
};

void ChartImagePreview::setChart(vtkstd::vector<vtkStdString>& units, btk::VTKChartTimeSeries* chart)
{
  this->mp_Chart->ClearPlots();
  this->mp_Chart->SetEventsFunctor(chart->GetEventsFunctor());
  this->mp_Chart->SetDisplayEvents(chart->GetDisplayEvents());
  units.resize(chart->GetNumberOfPlots());
  for (int i = 0 ; i < chart->GetNumberOfPlots() ; ++i)
  {
    vtkPlotLine* targetLine = vtkPlotLine::New();
    vtkPlot* sourceLine = chart->GetPlot(i);
    targetLine->SetInput(sourceLine->GetInput(), 0, 1);
    targetLine->SetLabel(sourceLine->GetLabel());
    targetLine->SetWidth(sourceLine->GetWidth());
    double color[3]; sourceLine->GetColor(color); targetLine->SetColor(color[0], color[1], color[2]);
    this->mp_Chart->AddPlot(targetLine);
    vtkStdString title = chart->GetPlot(i)->GetLabel();
    units[i] = title.substr(title.find(" ("));
  }
  
  vtkAxis* sourceAxisX = chart->GetAxis(vtkAxis::BOTTOM);
  vtkAxis* targetAxisX = this->mp_Chart->GetAxis(vtkAxis::BOTTOM);
  targetAxisX->SetTitle(sourceAxisX->GetTitle());
  targetAxisX->SetRange(sourceAxisX->GetMinimum(), sourceAxisX->GetMaximum());
  
  vtkAxis* sourceAxisY = chart->GetAxis(vtkAxis::LEFT);
  vtkAxis* targetAxisY = this->mp_Chart->GetAxis(vtkAxis::LEFT);
  targetAxisY->SetTitle(sourceAxisY->GetTitle());
  targetAxisY->SetRange(sourceAxisY->GetMinimum(), sourceAxisY->GetMaximum());
};

void ChartImagePreview::keyPressEvent(QKeyEvent* event)
{
  event->accept(); // Keyboard events are not sent to VTK
  // this->QVTKWidget::keyPressEvent(event);
};

void ChartImagePreview::keyReleaseEvent(QKeyEvent* event)
{
  event->accept(); // Keyboard events are not sent to VTK
  // this->QVTKWidget::keyReleaseEvent(event);
};

void ChartImagePreview::mousePressEvent(QMouseEvent* event)
{
  event->accept(); // Keyboard events are not sent to VTK
  // this->QVTKWidget::mousePressEvent(event);
};