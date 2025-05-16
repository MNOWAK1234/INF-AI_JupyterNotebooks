import android.annotation.SuppressLint
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.SearchBar
import androidx.compose.material3.SearchBarDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.navigation.NavController
import com.example.HikingTrails.Trail
import com.example.HikingTrails.ui.theme.LightGreen40
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.combine
import kotlinx.coroutines.flow.stateIn

@OptIn(ExperimentalMaterial3Api::class)
@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@Composable
fun SearchbarScreen(searchbarView: SearchbarView, searchText: String, isSearching: Boolean, searchbarTrailList: List<Trail>, navController: NavController, showPopupChange: (Boolean) -> Unit){
    SearchBar(
        query = searchText,
        onQueryChange = searchbarView::onSearchTextChange,
        onSearch = searchbarView::onSearchTextChange,
        active = isSearching,
        onActiveChange = { searchbarView.onToggleSearch() },
        modifier = Modifier
            .fillMaxWidth()
            .padding(16.dp),
        colors = SearchBarDefaults.colors(
            containerColor = Color.White
        ),
        placeholder = {
            Text(
                text = "Type to search for a trail",
                color = Color.Black
            )
        }
    ) {
        LazyColumn {
            items(searchbarTrailList) { trail ->
                Row(modifier = Modifier.clickable{
                    navController.navigate("LayoutPhoneDetailHorizontalRight/${trail.id}")
                    showPopupChange(false)
                }){
                    Text(
                        text = trail.name,
                        modifier = Modifier
                            .padding(
                                start = 8.dp,
                                top = 4.dp,
                                end = 8.dp,
                                bottom = 4.dp
                            )
                    )
                }
            }
        }
    }
}

@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@Composable
fun SearchbarContent(searchbarView: SearchbarView, navController: NavController, showPopupChange: (Boolean) -> Unit){
    val searchText by searchbarView.searchText.collectAsState()
    val isSearching by searchbarView.isSearching.collectAsState()
    val searchbarTrailList by searchbarView.trailList.collectAsState()
    SearchbarScreen(searchbarView = searchbarView, searchText = searchText, isSearching = isSearching, searchbarTrailList = searchbarTrailList, navController = navController, showPopupChange = showPopupChange)
}

class SearchbarView(trailList: List<Trail>) : ViewModel() {
    private val _isSearching = MutableStateFlow(false)
    val isSearching = _isSearching.asStateFlow()
    private val _searchText = MutableStateFlow("")
    val searchText = _searchText.asStateFlow()
    private val _trailList = MutableStateFlow(trailList)
    val trailList = searchText
        .combine(_trailList) { text, trails ->
            if (text.isBlank()) { //show all
                trails
            }
            trails.filter { trail ->// filter
                trail.name.uppercase().contains(text.trim().uppercase())
            }
        }.stateIn(
            scope = viewModelScope,
            started = SharingStarted.WhileSubscribed(6000),
            initialValue = _trailList.value
        )

    fun onSearchTextChange(text: String) {
        _searchText.value = text
    }

    fun onToggleSearch() {
        _isSearching.value = !_isSearching.value
        if (!_isSearching.value) {
            onSearchTextChange("")
        }
    }
}
