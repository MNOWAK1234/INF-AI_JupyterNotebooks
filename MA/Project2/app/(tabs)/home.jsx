import { useState } from "react";
import { SafeAreaView } from "react-native-safe-area-context";
import { FlatList, RefreshControl, Text, View } from "react-native";

import CercoSVG from "../../assets/images/cerco";
import useAppwrite from "../../lib/useAppwrite";
import { useGlobalContext } from "../../context/GlobalProvider";
import { getAllPosts, getLatestPosts } from "../../lib/appwrite";
import { EmptyState, SearchInput, Trending, VideoCard } from "../../components";

const Home = () => {
  const { data: posts, refetch } = useAppwrite(getAllPosts);
  const { data: latestPosts } = useAppwrite(getLatestPosts);
  const { user, setUser, setIsLogged } = useGlobalContext();

  const [refreshing, setRefreshing] = useState(false);

  const onRefresh = async () => {
    setRefreshing(true);
    await refetch();
    setRefreshing(false);
  };

  return (
    <SafeAreaView className="bg-primary h-full">
      <FlatList
        data={posts}
        keyExtractor={(item) => item.$id}
        renderItem={({ item }) => (
          <VideoCard
            title={item.title}
            thumbnail={item.thumbnail}
            video={item.video}
            creator={item.creator.username}
            avatar={item.creator.avatar}
          />
        )}
        ListHeaderComponent={() => (
          <View className="flex my-6 px-4 space-y-6">
            <View className="flex justify-between items-start flex-row mb-6">
              <View>
                <Text className="font-pmedium text-sm text-gray-100">
                  Hello
                </Text>
                <Text className="text-2xl font-psemibold text-white">
                  {user?.username}
                </Text>
              </View>

              <CercoSVG width={50} height={50} />
            </View>

            <SearchInput />

            <View className="w-full flex-1 pt-5 pb-8">
              <Text className="text-lg font-pregular text-gray-100 mb-3">
                Latest Videos
              </Text>

              <Trending posts={latestPosts ?? []} />
            </View>
          </View>
        )}
        ListEmptyComponent={() => (
          <EmptyState
            title="No Videos Found"
            subtitle="No videos created yet"
          />
        )}
        refreshControl={
          <RefreshControl refreshing={refreshing} onRefresh={onRefresh} />
        }
      />
    </SafeAreaView>
  );
};

export default Home;
