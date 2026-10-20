import React from 'react';
import { StyleSheet, View, useWindowDimensions, StatusBar } from 'react-native';

interface Props {
  children: React.ReactNode;
}

export const ResponsiveContainer: React.FC<Props> = ({ children }) => {
  const { width } = useWindowDimensions();
  const isDesktop = width >= 768;

  return (
    <View style={styles.outerBackground}>
      <StatusBar barStyle="light-content" />
      <View style={[styles.innerContainer, isDesktop && styles.desktopContainer]}>
        {children}
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  outerBackground: {
    flex: 1,
    backgroundColor: '#05050A',
    alignItems: 'center',
    justifyContent: 'center',
  },
  innerContainer: {
    flex: 1,
    width: '100%',
    backgroundColor: '#12121A',
  },
  desktopContainer: {
    maxWidth: 600,
    maxHeight: 920,
    borderRadius: 16,
    overflow: 'hidden',
    borderWidth: 1.5,
    borderColor: '#00F0FF33',
    shadowColor: '#00F0FF',
    shadowOffset: { width: 0, height: 4 },
    shadowOpacity: 0.3,
    shadowRadius: 12,
    marginVertical: 20,
  },
});
