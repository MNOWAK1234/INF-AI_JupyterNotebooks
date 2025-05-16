import React, { useState, useEffect, useRef } from "react";
import { View, Text, TextInput, TouchableOpacity, ScrollView, KeyboardAvoidingView, Platform, Alert } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import axios from 'axios';

const apiKey = '';

const AI = () => {
  const [messages, setMessages] = useState([]);
  const [inputMessage, setInputMessage] = useState("");
  const scrollViewRef = useRef();

  const sendMessage = async () => {
    if (inputMessage.trim() !== "") {
      const userMessage = { text: inputMessage, sender: "user" };
      setMessages(prevMessages => [...prevMessages, userMessage]); // Use functional update to ensure you use the latest state

      try {
        const promptMessages = messages.map(msg => ({ role: msg.sender, content: msg.text }));
        promptMessages.push({ role: "user", content: inputMessage });
        const response = await axios.post('https://api.openai.com/v1/chat/completions', {
          model: "gpt-3.5-turbo",
          messages: [{ role: "user", content: inputMessage }],
          temperature: 0.7
        }, {
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${apiKey}`,
          }
        });

        const botMessage = { text: response.data.choices[0].message.content, sender: "bot" };
        setMessages(prevMessages => [...prevMessages, botMessage]); // Use functional update to ensure you use the latest state
      } catch (error) {
        console.error('Error fetching response from OpenAI:', error);
        const botErrorMessage = { text: "OpenAI is currently unavailable", sender: "bot" };
        setMessages(prevMessages => [...prevMessages, botErrorMessage]); // Use functional update to ensure you use the latest state
      }

      setInputMessage("");
    }
  };

  const clearMessages = () => {
    Alert.alert(
      "Clear All Messages",
      "Are you sure you want to clear all messages?",
      [
        { text: "Cancel", onPress: () => {}, style: "cancel" },
        {
          text: "Clear All",
          onPress: () => {
            setMessages([]);
          },
        },
      ]
    );
  };

  useEffect(() => {
    // Scroll to the bottom when messages change
    scrollViewRef.current.scrollToEnd({ animated: true });
  }, [messages]);

  return (
    <SafeAreaView className="bg-primary h-full">
      <View className="flex-row items-center justify-between px-4 pt-6">
        <Text className="text-2xl text-white font-psemibold">AI (beta)</Text>
        <TouchableOpacity onPress={clearMessages} className="bg-red-600 rounded-full p-2 ml-4">
          <Text className="text-white">Clear All</Text>
        </TouchableOpacity>
      </View>
      <View className="flex-1 mt-4">
        <ScrollView className="flex-1 px-4" ref={scrollViewRef}>
          {messages.map((message, index) => (
            <View
              key={index}
              className={`p-2 my-2 rounded-lg ${
                message.sender === "user" ? "bg-blue-600 self-end" : "bg-gray-700 self-start"
              }`}
            >
              <Text className="text-white">{message.text}</Text>
            </View>
          ))}
        </ScrollView>
      </View>
      <KeyboardAvoidingView behavior={Platform.OS === "ios" ? "padding" : "height"} keyboardVerticalOffset={90}>
        <View className="flex-row items-center mb-4 px-4">
          <TextInput
            value={inputMessage}
            onChangeText={setInputMessage}
            placeholder="Type your message..."
            placeholderTextColor="#888"
            className="flex-1 bg-gray-200 rounded-lg p-2"
          />
          <TouchableOpacity onPress={sendMessage} className="bg-blue-600 p-3 rounded-lg ml-2">
            <Text className="text-white">Send</Text>
          </TouchableOpacity>
        </View>
      </KeyboardAvoidingView>
    </SafeAreaView>
  );
};

export default AI;
