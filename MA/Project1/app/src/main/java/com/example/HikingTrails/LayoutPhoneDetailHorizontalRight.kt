package com.example.HikingTrails

import android.net.Uri
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.core.content.FileProvider
import coil.compose.rememberImagePainter

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun LayoutPhoneDetailHorizontalRight(
    index: Int,
    modifier: Modifier,
    database: AppDatabase,
    timerViewModel: TimerViewModel
) {
    val context = LocalContext.current
    val file = remember { context.createImageFile() }
    val uri = remember { FileProvider.getUriForFile(context, "${BuildConfig.APPLICATION_ID}.provider", file) }
    var capturedImageUri by remember { mutableStateOf<Uri?>(null) }

    val cameraLauncher = rememberLauncherForActivityResult(ActivityResultContracts.TakePicture()) { isSuccess ->
        if (isSuccess) {
            capturedImageUri = uri
        } else {
            // Handle image capture failure
        }
    }

    val permissionLauncher = rememberLauncherForActivityResult(ActivityResultContracts.RequestPermission()) { isGranted ->
        if (isGranted) {
            cameraLauncher.launch(uri)
        } else {
            // Handle permission denied
        }
    }
    var trailInfo by remember { mutableStateOf(database.TrailDAO().getTrailDescription(index)) }
    var trailTimesList by remember { mutableStateOf(database.TimesDAO().getAllTimesForOneTrail(index)) }
    val trailImage = trailImageList.find { it.id == index }
    var timeModifier = remember { database.GlobalVariablesDAO().getTimeModifier() }
    LaunchedEffect(index) {
        trailInfo = database.TrailDAO().getTrailDescription(index)
        trailTimesList = database.TimesDAO().getAllTimesForOneTrail(index)
        timeModifier = database.GlobalVariablesDAO().getTimeModifier()
    }
    Surface(color = Color.Black) {
        Scaffold(
            floatingActionButton = {
                FloatingActionButton(
                    onClick = { permissionLauncher.launch(android.Manifest.permission.CAMERA) },
                    modifier = Modifier.padding(15.dp)
                ) {
                    Icon(Icons.Filled.Add, "Floating action button.", tint = Color.Green)
                }
            }
        ) { innerPadding ->
            Surface(color = Color.Black) {
                LazyColumn(
                    modifier = Modifier
                        .padding(innerPadding)
                        .fillMaxSize(),
                    verticalArrangement = Arrangement.spacedBy(4.dp)
                ) {
                    item {
                        trailImage?.let {
                            Image(
                                painter = painterResource(id = it.imageResId),
                                contentDescription = null,
                                contentScale = ContentScale.FillWidth,
                                modifier = Modifier
                                    .fillMaxWidth()
                                    .height(200.dp)
                            )
                        }
                        Text(
                            trailInfo.name,
                            modifier = modifier,
                            fontSize = 36.sp,
                            textAlign = TextAlign.Center,
                            color = Color.Green,
                            lineHeight = 48.sp
                        )
                        Text(
                            trailInfo.description,
                            modifier = modifier.padding(10.dp),
                            fontSize = 18.sp,
                            textAlign = TextAlign.Justify,
                            color = Color.Green
                        )
                        Text(
                            text = "Distance: ${trailInfo.distance}km",
                            modifier = Modifier
                                .fillMaxWidth()
                                .padding(5.dp),
                            fontSize = 30.sp,
                            color = Color.Green
                        )
                        Text(
                            text = "Time: ${(trailInfo.time * timeModifier / 3).toInt()} minutes",
                            modifier = Modifier
                                .fillMaxWidth()
                                .padding(5.dp),
                            fontSize = 30.sp,
                            color = Color.Green
                        )
                        TimerScreenContent(
                            timerViewModel = timerViewModel,
                            database = database,
                            TrailID = index
                        )
                        if (capturedImageUri?.path?.isNotEmpty() == true) {
                            Image(
                                modifier = Modifier.padding(16.dp, 8.dp),
                                painter = rememberImagePainter(capturedImageUri!!),
                                contentDescription = null
                            )
                        }
                    }
                    item {
                        Text(
                            "Previous Results:",
                            modifier = Modifier.fillMaxWidth(),
                            fontSize = 30.sp,
                            textAlign = TextAlign.Center,
                            color = Color.Green
                        )
                    }
                    items(trailTimesList.size) { index ->
                        val date = trailTimesList[index].date
                        val time = timeToText(trailTimesList[index].time)
                        Text(
                            "$date: $time",
                            modifier = Modifier.padding(horizontal = 8.dp),
                            color = Color.Green
                        )
                    }
                }
            }
        }
    }
}
