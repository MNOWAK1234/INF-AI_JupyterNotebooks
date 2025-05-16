import React, { useState, useEffect } from "react";
import { View, Text, Alert } from "react-native";
import * as LocalAuthentication from "expo-local-authentication";
import { CustomButton } from "../components";
import LottieView from 'lottie-react-native'; // Import Lottie library

const FingerprintAuthButton = () => {
  const [errorMessage, setErrorMessage] = useState('');
  const [animationVisible, setAnimationVisible] = useState(false);
  const [authenticated, setAuthenticated] = useState(false); // Track authentication status

  useEffect(() => {
    checkFingerprintAvailable();
  }, []);

  const checkFingerprintAvailable = async () => {
    const isEnrolled = await LocalAuthentication.hasHardwareAsync();
    if (!isEnrolled) {
      setErrorMessage('Biometric hardware not available or not enrolled');
    }
  };

  const handleFingerprintPress = async () => {
    try {
      if (authenticated) { // If already authenticated, hide animation and set button text
        setAnimationVisible(false);
        setAuthenticated(false);
      } else {
        const result = await LocalAuthentication.authenticateAsync({
          promptMessage: 'Authenticate',
          fallbackLabel: 'Enter Password',
        });
        if (result.success) {
          setAnimationVisible(true); // Show animation on successful authentication
          setAuthenticated(true); // Set authenticated status
        } else {
          setErrorMessage(result.error);
        }
      }
    } catch (error) {
      setErrorMessage(error.message);
    }
  };

  return (
    <>
      <CustomButton
        title={authenticated ? "Stop" : "Call private assistant"} // Change button text based on authentication status
        handlePress={handleFingerprintPress}
        containerStyles="w-full mt-7"
      />
      {animationVisible && (
        <>
          <LottieView
            source={require('../assets/images/animation.json')}
            autoPlay
            loop={true}
            style={{ width: 200, height: 200 }}
          />
          <Text style={{ color: 'white', textAlign: 'center', marginTop: 10, fontSize: 16 }}>
            Waiting for your private assistant...
          </Text>
          <Text style={{ color: 'white', textAlign: 'center', fontSize: 14 }}>
            Buy premium to reduce your waiting time
          </Text>
        </>
      )}
    </>
  );
};

export default FingerprintAuthButton;
