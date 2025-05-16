package com.example.HikingTrails

import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import android.net.Uri
import androidx.activity.compose.ManagedActivityResultLauncher
import androidx.core.content.ContextCompat
import java.io.File
import java.text.SimpleDateFormat
import java.util.Date
import android.Manifest

fun checkPermission(
        context: Context,
        cameraLauncher: ManagedActivityResultLauncher<Uri, Boolean>,
        uri: Uri,
        permissionLauncher: ManagedActivityResultLauncher<String, Boolean>){
    val permissionCheckResult =
        ContextCompat.checkSelfPermission(context, Manifest.permission.CAMERA)
    if (permissionCheckResult == PackageManager.PERMISSION_GRANTED) {
        cameraLauncher.launch(uri)
    } else {
        permissionLauncher.launch(Manifest.permission.CAMERA)
    }
}

@SuppressLint("SimpleDateFormat")
fun Context.createImageFile(): File {
    val timeStamp = SimpleDateFormat("yyyyMMdd_HHmmss").format(Date())
    val imageFileName = "JPEG_" + timeStamp + "_"
    val image = File.createTempFile(
        imageFileName,
        ".jpg",
        externalCacheDir
    )
    return image
}

@SuppressLint("DefaultLocale")
fun timeToText(time: Long): String{
    return String.format("%02d:%02d:%02d", time / 3600, (time % 3600) / 60, time % 60)
}
