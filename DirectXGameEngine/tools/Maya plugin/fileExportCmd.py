# import maya
# maya.cmds.loadPlugin("helloWorldCmd.py")
# fileExportCmd.py

# To load plugin, menu Window | Settings/Perferences | Plugin Manager.
# Scroll through the list and select plugin to auto load
# Only click on load to load plugin. Click again to unload. You'll need to 
# load and unload the plugin to see code changes

# put this file in C:\Program Files\Autodesk\Maya<year>\bin\plug-ins

import sys
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaRender as OpenMayaRender
import fileExportHeader as header
import array
import maya.cmds as cmds

# Unique Lists
uniqueVertAry = OpenMaya.MFloatPointArray()
uniqueNormAry = OpenMaya.MFloatVectorArray()
uniqueUAry = OpenMaya.MFloatArray()
uniqueVAry = OpenMaya.MFloatArray()
uniqueMaterialPathAry = list()
faceGrpList = list()

#cmds.confirmDialog( title='Confirm', message='Are you sure?', button=['Yes','No'], defaultButton='Yes', cancelButton='No', dismissString='No' )

kPluginCmdName = "myExport"

class CFaceGrp:
	# define as self so that these arrays are unique instances
	def __init__(self):
		self.vertAry = OpenMaya.MFloatPointArray()
		self.normAry = OpenMaya.MFloatVectorArray()
		self.uAry = OpenMaya.MFloatArray()
		self.vAry = OpenMaya.MFloatArray()
	
		self.vertIndex = OpenMaya.MIntArray()
		self.normIndex = OpenMaya.MIntArray()
		self.uvIndex = OpenMaya.MIntArray()
	
		self.matIndex = 0

# command
class scriptedCommand(OpenMayaMPx.MPxCommand):

	def __init__(self):
		OpenMayaMPx.MPxCommand.__init__(self)

	def doIt(self,argList):
		self.clearGlobalLists()
		self.buildLists()
		self.writeToFile()

	# Clear the global lists
	def clearGlobalLists(self):
		del faceGrpList[:]
		del uniqueMaterialPathAry[:]
		uniqueVertAry.clear()
		uniqueNormAry.clear()
		uniqueUAry.clear()
		uniqueVAry.clear()

	# Format the float point number output
	def format(self, value):
		return "%.6f" % value

	# Find the offset of the different paths
	def filePathOffsetDif(self, path1, path2):
		offset = 0

		# Go through the scene path and texture path to find the spot
		# both paths stop being the same and that's the starting point 
		# of our truncated texture file path
		for z in range(len(path1)):
			if path1[z] != path2[z]:
				offset = z
				break

		return offset

	# Get the material index
	def getMaterialIndex(self, matPath):
		matIndex = -1

		for i in range(len(uniqueMaterialPathAry)):
			if uniqueMaterialPathAry[i] == matPath:
				matIndex = i
				break;

		return matIndex

	# Add the path to the list if it doesn't already exists.
	def addMaterialPathToList(self, matPath):
		found = 0

		for i in range(len(uniqueMaterialPathAry)):
			if uniqueMaterialPathAry[i] == matPath:
				found = 1
				break;

		if found == 0:
			uniqueMaterialPathAry.append(matPath)

	# Build the lists
	def buildLists(self):

		dagPath = OpenMaya.MDagPath()
		dagIter = OpenMaya.MItDag(OpenMaya.MItDag.kDepthFirst, OpenMaya.MFn.kMesh)

		mfileio = OpenMaya.MFileIO()
		sceneFilePath = mfileio.currentFile()

		# Loop through all mesh objects
		while not dagIter.isDone():

			dagIter.getPath(dagPath)
			dagNodeFn = OpenMaya.MFnDagNode(dagPath)

			# this object cannot be intermediate, and it can't be a transform.
			# Intermediate objects are special meshes that are not drawn used for mesh morphs or something.
			if (dagNodeFn.isIntermediateObject() == 0) and (dagPath.hasFn( OpenMaya.MFn.kTransform ) == 0):
				
				# get the mesh
				fnMash = OpenMaya.MFnMesh(dagPath)

				# setup the temp face class
				tmpFaceGrp = CFaceGrp()

				# Get all the points in this model
				fnMash.getPoints(tmpFaceGrp.vertAry, OpenMaya.MSpace.kWorld)#OpenMaya.MSpace.kWorld OpenMaya.MSpace.kObject

				# Check for duplicates and add to the global list
				for i in range(tmpFaceGrp.vertAry.length()):

					# reset the point if any scaling was done
					tmpFaceGrp.vertAry[i].cartesianize()

					# convert to left hand system
					tmpFaceGrp.vertAry[i].z = -tmpFaceGrp.vertAry[i].z

					duplicateFound = 0

					for j in range(uniqueVertAry.length()):
						if uniqueVertAry[j] == tmpFaceGrp.vertAry[i]:
							duplicateFound = 1
							break

					if duplicateFound == 0:
						uniqueVertAry.append(tmpFaceGrp.vertAry[i])

				# Get all the vertex normals in this model
				fnMash.getNormals(tmpFaceGrp.normAry, OpenMaya.MSpace.kObject)

				# Check for duplicates and add to the global list
				for i in range(tmpFaceGrp.normAry.length()):

					# convert to left hand system
					tmpFaceGrp.normAry[i].z = -tmpFaceGrp.normAry[i].z

					duplicateFound = 0

					for j in range(uniqueNormAry.length()):
						if uniqueNormAry[j] == tmpFaceGrp.normAry[i]:
							duplicateFound = 1
							break

					if duplicateFound == 0:
						uniqueNormAry.append(tmpFaceGrp.normAry[i])

				sortUAry = OpenMaya.MFloatArray()
				sortVAry = OpenMaya.MFloatArray()

				# Get all the uv's in this model
				fnMash.getUVs(tmpFaceGrp.uAry, tmpFaceGrp.vAry)

				# Check for duplicates and add to the global list
				for i in range(tmpFaceGrp.uAry.length()):

					duplicateFound = 0

					for j in range(uniqueUAry.length()):
						if (uniqueUAry[j] == tmpFaceGrp.uAry[i]) and (uniqueVAry[j] == tmpFaceGrp.vAry[i]):
							duplicateFound = 1
							break

					if duplicateFound == 0:
						uniqueUAry.append(tmpFaceGrp.uAry[i])
						uniqueVAry.append(tmpFaceGrp.vAry[i])

				vertCount = OpenMaya.MIntArray()
				fnMash.getVertices(vertCount, tmpFaceGrp.vertIndex)

				normCount = OpenMaya.MIntArray()
				fnMash.getNormalIds(normCount, tmpFaceGrp.normIndex)

				uvCount = OpenMaya.MIntArray()
				fnMash.getAssignedUVs(uvCount, tmpFaceGrp.uvIndex)

				print "vert count: ", tmpFaceGrp.vertIndex.length(), " norm count: ", tmpFaceGrp.normIndex.length(), " uv count: ", tmpFaceGrp.uvIndex.length()

				for i in range(fnMash.parentCount()):

					dummy = OpenMaya.MIntArray()
					shader = OpenMaya.MObjectArray()

					fnMash.getConnectedShaders( i, shader, dummy )

					for j in range(shader.length()):

						# Get the shader node
						shaderNode = OpenMaya.MFnDependencyNode( shader[j] )

						# get access to the surfaceShader attribute. This will be 
						# connected to lambert , phong nodes etc.
						shaderPlug = shaderNode.findPlug("surfaceShader");

						# will hold the connections to the surfaceShader attribute
						material = OpenMaya.MPlugArray()

						# get the material connected to the surface shader
						shaderPlug.connectedTo( material, 1, 0 );

						for w in range(material.length()):

							# Get the material node
							materialNode = OpenMaya.MFnDependencyNode( material[w].node() )

							# Get the color plug
							colorPlug = materialNode.findPlug("color");

							# Need to use the dependency graph for this
							dgIt = OpenMaya.MItDependencyGraph(
												colorPlug,
												OpenMaya.MFn.kFileTexture,
												OpenMaya.MItDependencyGraph.kUpstream,
												OpenMaya.MItDependencyGraph.kBreadthFirst,
												OpenMaya.MItDependencyGraph.kNodeLevel )

							# not sure why we do this
							dgIt.disablePruningOnFilter()

							# Get the texture node
							textureNode = OpenMaya.MFnDependencyNode( dgIt.currentItem() )

							# Find the texture name plug
							texturePlug = textureNode.findPlug("fileTextureName")

							txtFilePath = texturePlug.asString()

							offset = self.filePathOffsetDif( sceneFilePath, txtFilePath )

							# Try to add the materila path to the list
							self.addMaterialPathToList( txtFilePath[offset:] )

							tmpFaceGrp.matIndex = self.getMaterialIndex( txtFilePath[offset:] )

							#print " name: ", materialNode.name(), ", path: ", txtFilePath[offset:], ", index: ", tmpFaceGrp.matIndex


				# add to our global face list
				faceGrpList.append(tmpFaceGrp)

			dagIter.next()

		#print "List done. vert Count: ", uniqueVertAry.length(), " norm Count: ", uniqueNormAry.length(), " uv count: ", uniqueUAry.length(), " face list count: ", len(faceGrpList)

		for item in faceGrpList:
			print " face items: ", item.vertIndex.length()

	# Write to file
	def writeToFile(self):

		#print " Object data"

		tagCheck = header.CTagCheck()

		tmpFilePath = OpenMaya.MFileIO().currentFile().rstrip("mb")
		newPath = tmpFilePath.replace("/", "\\")
		
		# open a file
		FILE = open(tmpFilePath + "rss", "wb")
		txtFile = open(tmpFilePath + "txt", 'w')

		fileHeader = header.CSpriteBinaryFileHeader()
		fileHeader.file_header = header.SPRITE_FILE_HEADER
		fileHeader.vert_count = uniqueVertAry.length()
		fileHeader.uv_count = uniqueUAry.length()
		fileHeader.vert_norm_count = uniqueNormAry.length()
		fileHeader.face_group_count = len(faceGrpList)
		fileHeader.mat_count = len(uniqueMaterialPathAry)
		fileHeader.joint_count = 0

		# write the file header
		FILE.write(fileHeader)
		print >>txtFile, "File Header: ", fileHeader.file_header, "\r"
		print >>txtFile, "File Verts: ", fileHeader.vert_count, "\r"
		print >>txtFile, "File UV: ", fileHeader.uv_count, "\r"
		print >>txtFile, "File Norm: ", fileHeader.vert_norm_count, "\r"
		print >>txtFile, "File Faces: ", fileHeader.face_group_count, "\r"
		print >>txtFile, "File Materials: ", fileHeader.mat_count, "\r"
		print >>txtFile, "File Joints: ", fileHeader.joint_count, "\r"

		# write the vert list tag check
		tagCheck.tagCheck = header.VERT_LIST;
		FILE.write(tagCheck)

		print " Vert List Count: ", uniqueVertAry.length()
		print >>txtFile, "\r\nVert List Count: ", fileHeader.vert_count, "\r"

		# write the verts
		for i in range(uniqueVertAry.length()):
			vert = header.CBinaryVert()
			vert.x = uniqueVertAry[i].x
			vert.y = uniqueVertAry[i].y
			vert.z = uniqueVertAry[i].z
			FILE.write(vert)
			print >>txtFile, "  x: ", self.format(vert.x), "  y: ", self.format(vert.y), "  z: ", self.format(vert.z), "\r"

		# write the uv list tag check
		tagCheck.tagCheck = header.UV_LIST;
		FILE.write(tagCheck)

		print " UV List Count: ", uniqueUAry.length()
		print >>txtFile, "\r\nUV List Count: ", fileHeader.uv_count, "\r"

		# write the uv
		for i in range(uniqueUAry.length()):
			uv = header.CBinaryUV()
			uv.u = uniqueUAry[i]
			uv.v = uniqueVAry[i]
			FILE.write(uv)
			print >>txtFile, "  u: ", self.format(uv.u), "  v: ", self.format(uv.v), "\r"

		# write the vert normal list tag check
		tagCheck.tagCheck = header.VERT_NORM_LIST;
		FILE.write(tagCheck)

		print " VNormal List Count: ", uniqueNormAry.length()
		print >>txtFile, "\r\nVNormal List Count: ", fileHeader.vert_norm_count, "\r"

		# write the normals
		for i in range(uniqueNormAry.length()):
			vert = header.CBinaryVert()
			vert.x = uniqueNormAry[i].x
			vert.y = uniqueNormAry[i].y
			vert.z = uniqueNormAry[i].z
			FILE.write(vert)
			print >>txtFile, "  x: ", self.format(vert.x), "  y: ", self.format(vert.y), "  z: ", self.format(vert.z), "\r"

		# write the face group tag check
		tagCheck.tagCheck = header.FACE_GROUP;
		FILE.write(tagCheck)

		print " Face Group Count: ", len(faceGrpList)
		print >>txtFile, "\r\nFace Group List Count: ", fileHeader.face_group_count, "\r"

		# write the face lists
		for i in range(len(faceGrpList)):
			faceGrp = faceGrpList[i]

			# write the face group tag check
			tagCheck.tagCheck = header.FACE_LIST;
			FILE.write(tagCheck)

			# all index lists should be the same size
			faceInfo = header.CBinaryFaceGroup()
			faceInfo.groupFaceCount = faceGrp.vertIndex.length() / 3
			faceInfo.matIndex = faceGrp.matIndex
			FILE.write(faceInfo)

			print " Face List Count: ", faceInfo.groupFaceCount, "; Material Index: ", faceInfo.matIndex
			print >>txtFile, "\r\nFace List Count: ", faceInfo.groupFaceCount, "; Material Index: ", faceGrp.matIndex, "\r"

			for j in range(faceInfo.groupFaceCount):
				fIndex = [0,0,0,0]
				bFace = header.CBinaryFace()

				# find the verts in the global list
				for w in range(3):
					found = 0
					vIndex = faceGrp.vertIndex[(j*3)+w]
					vert = faceGrp.vertAry[vIndex]

					for l in range(uniqueVertAry.length()):
						if vert == uniqueVertAry[l]:
							fIndex[w] = l
							found = 1
							break

					if found == 0:
						print "vert not found"

				# reorder for left handed system
				bFace.vert_1 = fIndex[2]
				bFace.vert_2 = fIndex[1]
				bFace.vert_3 = fIndex[0]

				# find the UV in the global list
				for w in range(3):
					found = 0
					uvIndex = faceGrp.uvIndex[(j*3)+w]
					u = faceGrp.uAry[uvIndex]
					v = faceGrp.vAry[uvIndex]

					for l in range(uniqueUAry.length()):
						if (u == uniqueUAry[l]) and (v == uniqueVAry[l]):
							fIndex[w] = l
							found = 1
							break

					if found == 0:
						print "uv not found"

				# reorder for left handed system
				bFace.uv_1 = fIndex[2]
				bFace.uv_2 = fIndex[1]
				bFace.uv_3 = fIndex[0]

				# find the vert normal in the global list
				for w in range(3):
					found = 0
					nIndex = faceGrp.normIndex[(j*3)+w]
					norm = faceGrp.normAry[nIndex]

					for l in range(uniqueNormAry.length()):
						if norm == uniqueNormAry[l]:
							fIndex[w] = l
							found = 1
							break

					if found == 0:
						print "norm not found"

				# reorder for left handed system
				bFace.norm_1 = fIndex[2]
				bFace.norm_2 = fIndex[1]
				bFace.norm_3 = fIndex[0]

				FILE.write(bFace)
				print >>txtFile, "  ", bFace.vert_1, ",", bFace.vert_2, ",", bFace.vert_3, "  ", bFace.uv_1, ",", bFace.uv_2, ",", bFace.uv_3, "  ", bFace.norm_1, ",", bFace.norm_2, ",", bFace.norm_3, "\r"

		# write the material tag check
		tagCheck.tagCheck = header.MAT_LIST;
		FILE.write(tagCheck)

		print " Material List Count: ", len(uniqueMaterialPathAry)
		print >>txtFile, "\r\nMaterial List Count: ", fileHeader.mat_count, "\r"

		materialPath = header.CMaterialPath()
		# Write the material paths
		for i in range(len(uniqueMaterialPathAry)):
			materialPath.path = uniqueMaterialPathAry[i]
			FILE.write(materialPath)
			print >>txtFile, " ", uniqueMaterialPathAry[i]


		FILE.close()
		txtFile.close()

		print "file write done"


# Creator
def cmdCreator():
	return OpenMayaMPx.asMPxPtr( scriptedCommand() )

# Initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.registerCommand( kPluginCmdName, cmdCreator )
	except:
		sys.stderr.write( "Failed to register command: %s\n" % kPluginCmdName )
		raise

# Uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.deregisterCommand( kPluginCmdName )
	except:
		sys.stderr.write( "Failed to unregister command: %s\n" % kPluginCmdName )
		raise

	
