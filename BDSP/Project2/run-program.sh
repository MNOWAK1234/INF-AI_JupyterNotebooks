#!/bin/bash

CLUSTER_NAME=$(/usr/share/google/get_metadata_value attributes/dataproc-cluster-name)

java -cp /usr/lib/kafka/libs/*:event-data-kafka-streams.jar \
com.example.bigdata.BicycleSharing ${CLUSTER_NAME}-w-0:9092 $1 $2 $3