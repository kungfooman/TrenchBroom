/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CreateBrushTool.h"
#include "Polyhedron.h"
#include "PreferenceManager.h"
#include "Preferences.h"
#include "Model/Brush.h"
#include "Model/BrushBuilder.h"
#include "Model/Layer.h"
#include "Model/World.h"
#include "Renderer/RenderService.h"
#include "View/MapDocument.h"

namespace TrenchBroom {
    namespace View {
        CreateBrushTool::CreateBrushTool(MapDocumentWPtr document) :
        Tool(false),
        m_document(document),
        m_polyhedron(NULL) {}
        
        CreateBrushTool::~CreateBrushTool() {
            delete m_polyhedron;
        }
        
        void CreateBrushTool::update(const BBox3& bounds) {
            delete m_polyhedron;
            m_polyhedron = new Polyhedron3(bounds);
            refreshViews();
        }

        void CreateBrushTool::performCreateBrush() {
            if (m_polyhedron != NULL) {
                MapDocumentSPtr document = lock(m_document);
                const Model::BrushBuilder builder(document->world(), document->worldBounds());
                Model::Brush* brush = builder.createBrush(*m_polyhedron, document->currentTextureName());
                
                const Transaction transaction(document, "Create brush");
                document->deselectAll();
                document->addNode(brush, document->currentLayer());
                document->select(brush);
                
                delete m_polyhedron;
                m_polyhedron = NULL;
            }
        }

        void CreateBrushTool::render(Renderer::RenderContext& renderContext, Renderer::RenderBatch& renderBatch) {
            if (m_polyhedron != NULL) {
                Renderer::RenderService renderService(renderContext, renderBatch);
                renderService.setForegroundColor(pref(Preferences::HandleColor));
                renderService.setLineWidth(2.0f);
                
                const Polyhedron3::EdgeList& edges = m_polyhedron->edges();
                Polyhedron3::EdgeList::ConstIterator it = edges.iterator();
                while (it.hasNext()) {
                    const Polyhedron3::Edge* edge = it.next();
                    renderService.renderLine(edge->firstVertex()->position(), edge->secondVertex()->position());
                }
            }
        }
    }
}
