import React from "react";
import { View, Text, ScrollView } from "react-native";
import { StatusBar } from "expo-status-bar";
import { SafeAreaView } from "react-native-safe-area-context";
import { Redirect, router } from "expo-router";

import { CustomButton, Loader } from "../components";
import { useGlobalContext } from "../context/GlobalProvider";
import CercoSVG from "../assets/images/cerco";

const Welcome = () => {
  const { loading, isLogged } = useGlobalContext();

  if (!loading && isLogged) return <Redirect href="/home" />;

  return (
    <SafeAreaView className="bg-primary h-full">
      <Loader isLoading={loading} />

      <ScrollView
        contentContainerStyle={{
          height: "100%",
        }}
      >
        <View className="w-full flex justify-center items-center h-full px-4">
          <CercoSVG width={300} height={300} />
          <View className="relative mt-5">
            <Text className="text-3xl text-white font-bold text-center">
              Explore the world{"\n"}
              and find yourself with{" "}
              <Text className="text-secondary-200">Cerco</Text>
            </Text>
          </View>

          <CustomButton
            title="Continue with Email"
            handlePress={() => router.push("/sign-in")}
            containerStyles="w-full mt-7"
          />
        </View>
      </ScrollView>
      <StatusBar backgroundColor='#161622' style='light'/>
    </SafeAreaView>
  );
};

export default Welcome;
