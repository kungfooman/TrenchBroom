/*
 Copyright (C) 2010-2013 Kristian Duske
 
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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__MeshRenderer__
#define __TrenchBroom__MeshRenderer__

#include "Renderer/VertexSpec.h"
#include "Renderer/Mesh.h"

namespace TrenchBroom {
    namespace Assets {
        class Texture;
    }
    
    namespace Model {
        class Entity;
    }
    
    namespace Renderer {
        class RenderContext;
        class VertexArray;
        
        class MeshRenderer {
        private:
            typedef std::map<const Assets::Texture*, VertexArray> VertexArrayMap;
            VertexArrayMap m_vertexArrays;
            bool m_prepared;
        public:
            template <typename VertexSpec>
            MeshRenderer(Vbo& vbo, const Mesh<const Assets::Texture*, VertexSpec>& mesh) :
            m_vertexArrays(mesh.triangleSetArrays(vbo)),
            m_prepared(false) {}
            
            void prepare();
            void render();
        };
    }
}

#endif /* defined(__TrenchBroom__MeshRenderer__) */
