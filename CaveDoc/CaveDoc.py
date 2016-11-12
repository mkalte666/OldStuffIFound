import os

		
#this class builds the file tree
class Indexer:
	def __init__(self, foldername):
		self.files = []
		for root, dirs, files in os.walk(foldername):
			for file in files:
				self.files.append(root+'\\'+file)
	def GetFiles(self):
		return self.files
		
class HTMLBuilder:
	