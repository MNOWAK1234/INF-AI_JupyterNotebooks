import React, { useEffect, useState, useRef } from "react";
import { Text, StyleSheet, View, ActivityIndicator, TouchableOpacity } from "react-native";
import { SafeAreaView } from "react-native-safe-area-context";
import MapView, { PROVIDER_GOOGLE, Marker, Polyline, Circle } from "react-native-maps";
import * as Location from 'expo-location';

const Map = () => {
  const [location, setLocation] = useState(null);
  const [errorMsg, setErrorMsg] = useState(null);
  const [heading, setHeading] = useState(null);
  const [loading, setLoading] = useState(true);
  const [secondMarker, setSecondMarker] = useState(null);
  const [redDots, setRedDots] = useState([]);
  const mapRef = useRef(null);

  useEffect(() => {
    (async () => {
      let { status } = await Location.requestForegroundPermissionsAsync();
      if (status !== 'granted') {
        setErrorMsg('Permission to access location was denied');
        setLoading(false);
        return;
      }

      // Get initial location
      let location = await Location.getCurrentPositionAsync({});
      setLocation(location.coords);
      setHeading(location.coords.heading);

      // Add a red dot
      addRedDot(location.coords.latitude, location.coords.longitude);

      // Reverse geocoding to get address
      let address = await Location.reverseGeocodeAsync({
        latitude: location.coords.latitude,
        longitude: location.coords.longitude
      });

      // Use the first address obtained to get coordinates
      if (address.length > 0) {
        let addressLocation = await Location.geocodeAsync(address[0].formattedAddress);
        if (addressLocation.length > 0) {
          setSecondMarker({
            latitude: addressLocation[0].latitude,
            longitude: addressLocation[0].longitude
          });
        }
      }

      setLoading(false);

      // Watch for location changes
      Location.watchPositionAsync(
        { accuracy: Location.Accuracy.High, timeInterval: 1000, distanceInterval: 1 },
        (newLocation) => {
          setLocation(newLocation.coords);
          setHeading(newLocation.coords.heading);
          addRedDot(newLocation.coords.latitude, newLocation.coords.longitude);
        }
      );
    })();
  }, []);

  const addRedDot = (latitude, longitude) => {
    setRedDots(prevDots => [...prevDots, { latitude, longitude }]);
  };

  const clearRedDots = () => {
    setRedDots([]);
  };

  return (
    <SafeAreaView style={styles.container}>
      {loading ? (
        <ActivityIndicator size="large" color="#1f2937" />
      ) : (
        <>
          <MapView
            ref={mapRef}
            style={styles.map}
            provider={PROVIDER_GOOGLE}
            initialRegion={{
              latitude: location ? location.latitude : 37.78825,
              longitude: location ? location.longitude : -122.4324,
              latitudeDelta: 0.0922,
              longitudeDelta: 0.0421
            }}
          >
            {location && (
              <>
                <Circle
                  center={{
                    latitude: location.latitude,
                    longitude: location.longitude
                  }}
                  radius={3} // Smaller radius
                  fillColor="rgba(13, 240, 46, 0.3)"
                  strokeColor="rgba(13, 240, 46, 0.5)"
                />
                {redDots.map((dot, index) => (
                  <Circle
                    key={index}
                    center={dot}
                    radius={0.3} // Small radius for dots
                    fillColor="red"
                    strokeColor="red"
                  />
                ))}
                {secondMarker && (
                  <Marker
                    coordinate={secondMarker}
                    title="Second Marker"
                    pinColor="red" // Red color
                  />
                )}
                {secondMarker && (
                  <Polyline
                    coordinates={[
                      { latitude: location.latitude, longitude: location.longitude },
                      { latitude: secondMarker.latitude, longitude: secondMarker.longitude }
                    ]}
                    strokeColor="gray" // Gray color
                    strokeWidth={2} // Adjust width
                    lineDashPattern={[4, 4]} // Dotted line pattern
                  />
                )}
              </>
            )}
          </MapView>
          <TouchableOpacity style={styles.clearButton} onPress={clearRedDots}>
            <Text style={styles.clearButtonText}>Clear Dots</Text>
          </TouchableOpacity>
        </>
      )}
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#161622', // Primary color from Tailwind CSS
  },
  map: {
    ...StyleSheet.absoluteFillObject,
  },
  clearButton: {
    position: 'absolute',
    bottom: 16,
    right: 16,
    backgroundColor: 'red',
    paddingVertical: 8,
    paddingHorizontal: 16,
    borderRadius: 5,
  },
  clearButtonText: {
    color: 'white',
    fontWeight: 'bold',
  }
});

export default Map;
