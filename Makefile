all: proteins segmentation segments
PC = ./src/Proteins_Clustering
RSC = ./src/RoadSegment_Clustering

proteins:
	mkdir -p ./build ./results
	
	g++ -o ./build/proteins_clustering $(PC)/Proteins_Clustering.cpp $(PC)/protein_main_functions.cpp \
	./src/Object_Info.cpp ./src/Distance_Metric.cpp ./src/Mean_Frechet.cpp \
	./src/Transform.cpp ./src/Initialization.cpp ./src/Cluster.cpp ./src/Update.cpp \
	./src/Assignment.cpp ./src/Silhouette.cpp -I ./include/Eigen

segmentation:
	g++ -o ./build/ways_segmentation $(RSC)/Road_Segmentation.cpp ./src/HashTable.cpp ./src/List.cpp \
	./src/Node.cpp ./src/HashFunctions.cpp ./src/generator.cpp ./src/Object_Info.cpp $(RSC)/Ways.cpp \
	$(RSC)/NodeWay.cpp $(RSC)/Road_Segmentation_Functions.cpp

segments:
	g++ -o ./build/segments_clustering $(RSC)/Segments_Clustering.cpp $(RSC)/Segments_main_functions.cpp \
	./src/HashTable.cpp ./src/List.cpp ./src/Node.cpp ./src/HashFunctions.cpp ./src/generator.cpp \
	./src/Object_Info.cpp $(RSC)/Ways.cpp $(RSC)/NodeWay.cpp $(RSC)/Road_Segmentation_Functions.cpp \
	./src/Distance_Metric.cpp ./src/Mean_Frechet.cpp ./src/Transform.cpp ./src/Initialization.cpp \
	./src/Cluster.cpp ./src/Update.cpp ./src/Assignment.cpp ./src/Silhouette.cpp -I ./include/Eigen

clean:
	rm -rf ./build/* ./results/*.dat