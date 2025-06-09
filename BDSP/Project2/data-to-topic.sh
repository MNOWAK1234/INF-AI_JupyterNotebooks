#!/bin/bash

CLUSTER_NAME=$(/usr/share/google/get_metadata_value attributes/dataproc-cluster-name)

java -cp /usr/lib/kafka/libs/*:TestProducer.jar \
com.example.bigdata.TestProducer bicycle_result 15 input-topic \
0 ${CLUSTER_NAME}-w-0:9092