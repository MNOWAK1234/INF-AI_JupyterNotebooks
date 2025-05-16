package com.example.HikingTrails

import android.annotation.SuppressLint
import android.util.Log
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.sp
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.Date

class TimerViewModel : ViewModel() {
    @SuppressLint("SimpleDateFormat")
    private val dateFormat = SimpleDateFormat("dd-MM-yyyy")
    private val miliseconds = MutableStateFlow(0L)
    private var trailID: Int = -1
    val timer = miliseconds.asStateFlow()

    private var timerJob: Job? = null

    fun startTimer(trailID: Int) {
        this.trailID = trailID
        timerJob?.cancel()
        timerJob = viewModelScope.launch {
            while (true) {
                delay(1000)
                miliseconds.value++
            }
        }
    }

    fun pauseTimer() {
        timerJob?.cancel()
    }

    fun stopTimer(database: AppDatabase) {
        database.TimesDAO().insert(Time(database.TimesDAO().genNextIndex(), trailID, miliseconds.value, dateFormat.format(Date())))
        miliseconds.value = 0
        timerJob?.cancel()
    }

    override fun onCleared() {
        super.onCleared()
        timerJob?.cancel()
    }
}

@Composable
fun TimerScreenContent(timerViewModel: TimerViewModel, database: AppDatabase, TrailID: Int) {
    val timerValue by timerViewModel.timer.collectAsState()

    TimerScreen(
        timerValue = timerValue,
        onStartClick = { timerViewModel.startTimer(TrailID) },
        onPauseClick = { timerViewModel.pauseTimer() },
        onStopClick = { timerViewModel.stopTimer(database = database) }
    )
}

@Composable
fun TimerScreen(
    timerValue: Long,
    onStartClick: () -> Unit,
    onPauseClick: () -> Unit,
    onStopClick: () -> Unit
) {
    Text(text = timeToText(timerValue), modifier = Modifier.fillMaxWidth(), fontSize = 24.sp, textAlign = TextAlign.Center, color = Color.Green)
    Row(modifier = Modifier.fillMaxSize(), verticalAlignment = Alignment.CenterVertically, horizontalArrangement = Arrangement.Center) {
        IconButton(onClick = onStartClick) {
            Icon(PlayArrow(), contentDescription = "Start", tint = Color.Green)
        }
        IconButton(onClick = onPauseClick) {
            Icon(PauseIcon(), contentDescription = "Pause", tint = Color.Green)
        }
        IconButton(onClick = onStopClick) {
            Icon(StopIcon(), contentDescription = "Stop", tint = Color.Green)
        }
    }
}