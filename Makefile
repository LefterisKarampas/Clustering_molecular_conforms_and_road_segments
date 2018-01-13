all: proteins segmentation segments
PC = ./src/Proteins_Clustering
RSC = ./src/RoadSegment_Clustering
DI = ./src/Distance
CL = ./src/Clustering
LSH= ./src/LSH
LIBS = -I ./include/Eigen


proteins:
	mkdir -p ./build ./results
	g++ -o ./build/proteins_clustering $(PC)/Proteins_Clustering.cpp $(PC)/protein_main_functions.cpp \
	$(CL)/Object_Info.cpp $(DI)/Distance_Metric.cpp $(CL)/Mean_Frechet.cpp \
	$(DI)/Transform.cpp $(CL)/Initialization.cpp $(CL)/Cluster.cpp $(CL)/Update.cpp \
	$(CL)/Assignment.cpp $(CL)/Silhouette.cpp $(CL)/Clustering.cpp $(LIBS)

segmentation:
	mkdir -p ./build
	g++ -o ./build/ways_segmentation $(RSC)/Road_Segmentation.cpp $(LSH)/HashTable.cpp $(LSH)/List.cpp \
	$(LSH)/Node.cpp $(LSH)/HashFunctions.cpp ./src/generator.cpp $(CL)/Object_Info.cpp $(RSC)/Ways.cpp \
	$(RSC)/NodeWay.cpp $(RSC)/Road_Segmentation_Functions.cpp

segments:
	mkdir -p ./build ./results 
	g++ -o ./build/segments_clustering $(RSC)/Segments_Clustering.cpp $(RSC)/Segments_main_functions.cpp \
	$(LSH)/HashTable.cpp $(LSH)/List.cpp $(LSH)/Node.cpp $(LSH)/HashFunctions.cpp ./src/generator.cpp \
	$(CL)/Object_Info.cpp $(RSC)/Ways.cpp $(RSC)/NodeWay.cpp $(RSC)/Road_Segmentation_Functions.cpp \
	$(DI)/Distance_Metric.cpp $(CL)/Mean_Frechet.cpp $(DI)/Transform.cpp $(CL)/Initialization.cpp \
	$(CL)/Cluster.cpp $(CL)/Update.cpp $(CL)/Assignment.cpp $(CL)/Silhouette.cpp \
	$(CL)/Clustering.cpp $(CL)/LSH_Clustering.cpp $(LSH)/LSH_HashTable.cpp $(LSH)/LSH.cpp \
	$(LSH)/Grid.cpp $(LIBS)

clean:
	rm -rf ./build/* ./results/*.dat