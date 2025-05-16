package com.example.HikingTrails

import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.animation.core.tween
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.size
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import kotlinx.coroutines.delay

sealed class Screens(val option: String) {
    data object Splash : Screens("splash_screen")
    data object Home : Screens("home_screen")
}

@Composable
fun AnimatedSplashScreen(navController: NavHostController) {
    var startAnimation by remember { mutableStateOf(false) }
    val alphaAnim = animateFloatAsState(
        targetValue = if (startAnimation) 1f else 0f,
        animationSpec = tween(
            durationMillis = 4000
        )
    )

    LaunchedEffect(key1 = true) {
        startAnimation = true
        delay(4000)
        navController.popBackStack()
        navController.navigate(Screens.Home.option)
    }
    Splash(jpgImage = R.drawable.splash, alpha = alphaAnim.value)
}

@Composable
fun Splash(jpgImage: Int, alpha: Float) {
    val configuration = LocalConfiguration.current
    val context = LocalContext.current

    val minDimension = minOf(configuration.screenWidthDp.dp, configuration.screenHeightDp.dp)
    val imageSize = minDimension * 1.0f // Adjust the factor as needed

    Box(
        modifier = Modifier
            .background(Color.Black)
            .fillMaxSize(),
        contentAlignment = Alignment.Center
    ) {
        Image(
            painter = painterResource(id = jpgImage),
            contentDescription = "Logo",
            modifier = Modifier
                .size(imageSize)
                .alpha(alpha = alpha),
            contentScale = ContentScale.Fit
        )
    }
}