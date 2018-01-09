all: proteins segmentation

proteins:
	mkdir -p ./build ./results
	
	g++ -o ./build/proteins_clustering ./src/Proteins_Clustering.cpp ./src/protein_main_functions.cpp \
	./src/Object_Info.cpp ./src/Distance.cpp ./src/Transform.cpp ./src/Initialization.cpp \
	./src/Cluster.cpp ./src/Update.cpp ./src/Assignment.cpp ./src/Silhouette.cpp -I ./include/Eigen

segmentation:
	g++ -o ./build/segmentation ./src/Road_Segmentation.cpp ./src/HashTable.cpp ./src/List.cpp \
	./src/Node.cpp ./src/HashFunctions.cpp ./src/generator.cpp ./src/Object_Info.cpp ./src/Ways.cpp \
	./src/Distance.cpp ./src/Transform.cpp -lm -I ./include/Eigen

roads:
	@echo "nothing yet"

clean:
	rm -rf ./build/* ./results/*.dat