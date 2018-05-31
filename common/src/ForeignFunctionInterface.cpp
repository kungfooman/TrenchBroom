#include "View/Console.h"

#include "FileLogger.h"
#include "View/ViewConstants.h"

#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/wupdlock.h>

#include <iostream>

#include "View/Selection.h"
#include "View/FrameManager.h"
#include "TrenchBroomApp.h"
#include "View/MapFrame.h"
#include "View/MapDocument.h"
#include "Model/Brush.h"

// ripped from Brush.cpp
#include "CollectionUtils.h"
#include "Model/BrushContentTypeBuilder.h"
#include "Model/BrushFace.h"
#include "Model/BrushGeometry.h"
#include "Model/BrushSnapshot.h"
#include "Model/Entity.h"
#include "Model/FindContainerVisitor.h"
#include "Model/FindGroupVisitor.h"
#include "Model/FindLayerVisitor.h"
#include "Model/Group.h"
#include "Model/IssueGenerator.h"
#include "Model/NodeVisitor.h"
#include "Model/PickResult.h"
#include "Model/World.h"

#include <Notifier.h>
#include <Renderer/MapRenderer.h>
#include <View/RenderView.h>
#include <View/SwitchableMapViewContainer.h>
#include <Renderer/MapRenderer.h>
#include <ForeignFunctionInterface.h>

#include "ForeignFunctionInterface.h"

using namespace TrenchBroom::View;
using namespace TrenchBroom::Model;
using namespace TrenchBroom;

extern TrenchBroom::Model::Brush *brushes[16384]; // 2 ^ 14, should be enough



extern wxTextCtrl *juliaTextWidgetOutput;

// atm just some helper functions, maybe turn them into ffi

MapFrame *firstMapFrame() {
	TrenchBroom::View::FrameManager *fm = TrenchBroom::View::TrenchBroomApp::instance().frameManager();
	auto frames = fm->frames();
	if (frames.size() == 0)
		return NULL;
	return frames.front();
}

MapDocumentSPtr getDocument() {
	return firstMapFrame()->document();
}

void print(char *format, ...) {
	char *buf;
	va_list args;
	va_start (args, format);
	// the output can be enourmous, so instead of a static buffer, we first figure out the length needed and then just alloc that size
	size_t needed = vsnprintf(NULL, 0, format, args);
	needed += 1;
	buf = (char *)malloc(needed);
	if (buf == NULL) {
		ffi_julia_print("failed to allocate memory for imgui_log()\n");
		return;
	}
	vsnprintf(buf, needed, format, args);
	va_end (args);
	ffi_julia_print(buf);
	free(buf);
}

CCALL void ffi_julia_print(char *message) {
	if (juliaTextWidgetOutput == NULL)
		return;
	if (juliaTextWidgetOutput->IsBeingDeleted()) return;
	wxWindowUpdateLocker locker(juliaTextWidgetOutput);
	const long start = juliaTextWidgetOutput->GetLastPosition();
	juliaTextWidgetOutput->AppendText(message);
	#ifndef __APPLE__
	juliaTextWidgetOutput->ScrollLines(5);
	#endif
}

// ccall( :testbrush, Void, ())
// I just use this to test around quickly
CCALL void ffi_testbrush() {
	TrenchBroom::View::FrameManager *fm = TrenchBroom::View::TrenchBroomApp::instance().frameManager();
	auto frames = fm->frames();
	for (MapFrame *frame : frames) {
		ffi_julia_print("frame\n");
		auto &brushes = frame->document()->selectedNodes().brushes();
		for (auto brush : brushes) {
			print("brush %d has faceCount=%d\n", brush->id, brush->faceCount());
		}	
	}
}

CCALL void *ffi_malloc(int bytes) {
	return malloc(bytes);
}

CCALL void ffi_free(void *ptr) {
	free(ptr);
}

CCALL int *ffi_selected_brushes() {
	auto frame = firstMapFrame();
	if (frame == NULL)
		return NULL;
	int n = frame->document()->selectedNodes().brushCount();
	int *ret = (int *)malloc(n * sizeof(int));
	if (ret == NULL)
		return NULL;
	auto &brushes = frame->document()->selectedNodes().brushes();
	for (int i=0; i<n; i++)
		ret[i] = brushes[i]->id;
	return ret;
}

CCALL int ffi_selected_brushes_count() {
	auto frame = firstMapFrame();
	if (frame == NULL)
		return -1;
	return frame->document()->selectedNodes().brushCount();
}

CCALL void ffi_update_views() {
	//Notifier1<const Model::NodeList&>::NotifyBeforeAndAfter notifyParents(&(doc->nodesWillChangeNotifier), nodesDidChangeNotifier, doc->selectedNodes());
	auto doc = getDocument();
	doc->invalidateSelectionBounds();
	firstMapFrame()->m_mapView->m_mapRenderer->invalidateRenderers(Renderer::MapRenderer::Renderer::Renderer_Selection);
	firstMapFrame()->m_mapView->m_mapRenderer->invalidateEntityLinkRenderer();
	firstMapFrame()->m_mapView->Refresh();
}

CCALL bool ffi_brush_rotate(int id, float roll, float pitch, float yaw) {
	Brush *brush = brushes[id];
	if (brushes[id] == NULL)
		return false;
	//auto mat = new Mat4x4();
	auto rot = rotationMatrix(roll, pitch, yaw);
	auto bounds = BBox3(16384);
	brush->transform(rot, true, bounds);
	return true;
}


CCALL bool ffi_add_texture_folder(const char *folder) {
	return false;
}


// includes ripped from TextureCollectionLoader.cpp
#include "IO/TextureCollectionLoader.h"
#include "Assets/AssetTypes.h"
#include "Assets/TextureCollection.h"
#include "Assets/TextureManager.h"
#include "IO/DiskIO.h"
#include "IO/FileMatcher.h"
#include "IO/FileSystem.h"
#include "IO/TextureReader.h"
#include "IO/WadFileSystem.h"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <IO/FreeImageTextureReader.h>
#include <IO/TextureReader.h>

#include <Assets/Texture.h>

extern TrenchBroom::IO::TextureCollectionLoader *textureCollectionLoaders[32];

#include "Assets/Texture.h"
#include "Assets/ImageUtils.h"
#include "Assets/TextureCollection.h"

extern int nextTextureCollectionID;
extern TrenchBroom::Assets::TextureCollection *textureCollections[256];

extern TrenchBroom::Assets::TextureManager *textureManagers[32];

// ccall( :ffi_add_texture, Bool, ())
CCALL bool ffi_add_texture(/*int texture_collection_loader_id, int texture_collection_id*/) {
	auto texreader = new IO::FreeImageTextureReader(IO::TextureReader::TextureNameStrategy());
	auto texpath = IO::Path("zzz.jpg");

	FILE *f = fopen("C:\\Users\\kung\\Desktop\\cc0 images\\flower-pink-daisy_powerscaled.jpg", "rb");
	assert(f);
	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buffer = (char *) malloc(length);
	fread(buffer, 1, length, f);
	fclose(f);

	TrenchBroom::Assets::Texture *tex = texreader->readTexture(buffer, buffer+length, texpath);

	/*
		probably should use the preference, but idc atm
            PreferenceManager& prefs = PreferenceManager::instance();
            prefs.set(Preferences::TextureMinFilter, minFilter);
            prefs.set(Preferences::TextureMagFilter, magFilter);
	*/
	int texid;
	TrenchBroom::glGenTextures((TrenchBroom::GLsizei) 1, (TrenchBroom::GLuint *)&texid);
	tex->prepare(texid, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);

	textureCollections[1]->addTexture(tex);

	textureManagers[0]->prepare();
	textureManagers[0]->updateTextures();

	return true;
}
