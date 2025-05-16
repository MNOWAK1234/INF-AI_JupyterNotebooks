package com.example.HikingTrails

fun populateDatabase(database: AppDatabase) {
    val trailCount = database.TrailDAO().getTrailCount()
    if (trailCount == 0) {
        // Trail 0
        database.TrailDAO().insert(
            Trail(
                0,
                "Mystical Mountain Trail",
                7.5,
                68,
                "Embark on a mystical journey to the peak of Kasprowy Wierch from Kuźnice. This trail, while serene at times, offers stunning views and a tranquil escape from the bustling crowds.",
                1
            )
        )

        // Trail 1
        database.TrailDAO().insert(
            Trail(
                1,
                "Charming Creek Walk",
                6.0,
                77,
                "Enjoy a charming walk through the scenic Kościeliska Valley. This trail is perfect for nature lovers seeking a peaceful retreat amidst lush greenery.",
                1
            )
        )

        // Trail 2
        database.TrailDAO().insert(
            Trail(
                2,
                "Meadow Path Adventure",
                7.2,
                112,
                "Experience an adventure through the picturesque meadows of Polana Chochołowska. While less challenging, this trail offers serene beauty and a chance to reconnect with nature.",
                1
            )
        )

        // Trail 3
        database.TrailDAO().insert(
            Trail(
                3,
                "Rusinowa Polana Expedition",
                9.0,
                345,
                "Embark on a long but rewarding journey through the Rusinowa Polana. This trail offers two distinct paths, each showcasing the beauty of the Tatras in its own way.",
                2
            )
        )

        // Trail 4
        database.TrailDAO().insert(
            Trail(
                4,
                "Chłopek Pass Trek",
                4.0,
                65,
                "Challenge yourself on the rugged Chłopek Pass trail, known for its breathtaking views and rocky terrain. While shorter, this trail offers a thrilling adventure for experienced hikers.",
                2
            )
        )

        // Trail 5
        database.TrailDAO().insert(
            Trail(
                5,
                "Nosal Peak Quest",
                1.8,
                89,
                "Embark on a quest to conquer Nosal Peak from Kuźnice. Despite its proximity to Zakopane, this trail offers a rewarding challenge and stunning panoramic views.",
                2
            )
        )

        // Trail 6
        database.TrailDAO().insert(
            Trail(
                6,
                "Zakopane Adventure Trail",
                3.5,
                90,
                "Explore the breathtaking landscapes surrounding Zakopane on this adventurous trail. From lush forests to rocky peaks, this trail offers a taste of everything the Tatras have to offer.",
                3
            )
        )

        // Trail 7
        database.TrailDAO().insert(
            Trail(
                7,
                "Giewont Summit Trek",
                12.0,
                123,
                "Conquer the iconic Giewont Summit on this challenging trek. With its steep ascent and rocky terrain, this trail offers a thrilling adventure for experienced hikers.",
                3
            )
        )

        // Trail 8
        database.TrailDAO().insert(
            Trail(
                8,
                "Sunset Ridge Trail",
                8.5,
                234,
                "Experience the beauty of a Tatra sunset on the Sunset Ridge Trail. This moderate trek offers stunning views and tranquil surroundings, perfect for nature enthusiasts.",
                3
            )
        )

        // Trail 9
        database.TrailDAO().insert(
            Trail(
                9,
                "Enchanted Forest Walk",
                5.0,
                56,
                "Immerse yourself in the enchanting beauty of the Tatra forest on this leisurely walk. With its gentle terrain and serene surroundings, this trail is perfect for a relaxing stroll.",
                1
            )
        )

        // Trail 10
        database.TrailDAO().insert(
            Trail(
                10,
                "Snowy Summit Expedition",
                10.0,
                73,
                "Embark on an epic expedition to the snowy summit of Rysy. This challenging trail offers breathtaking views and a true test of endurance for seasoned hikers.",
                4
            )
        )

        // Trail 11
        database.TrailDAO().insert(
            Trail(
                11,
                "Frozen Waterfall Trail",
                6.8,
                180,
                "Discover the beauty of frozen waterfalls on this winter trail. As you trek through snow-covered landscapes, you'll be greeted by the magical sight of icy cascades.",
                2
            )
        )

        // Trail 12
        database.TrailDAO().insert(
            Trail(
                12,
                "Misty Mountain Path",
                9.5,
                145,
                "Embark on a journey through mist-shrouded peaks and tranquil valleys. This trail offers a mystical atmosphere and breathtaking views at every turn.",
                3
            )
        )

        // Trail 13
        database.TrailDAO().insert(
            Trail(
                13,
                "Hidden Valley Hike",
                8.0,
                234,
                "Explore a hidden valley tucked away in the heart of the Tatras. With its secluded trails and untouched beauty, this hike is perfect for adventurers seeking solitude.",
                3
            )
        )

        // Trail 14
        database.TrailDAO().insert(
            Trail(
                14,
                "Crystal Lake Loop",
                5.5,
                120,
                "Journey to the shimmering Crystal Lake on this scenic loop trail. Surrounded by towering peaks and pristine forests, this trail offers a refreshing escape from the city.",
                2
            )
        )

        // Trail 15
        database.TrailDAO().insert(
            Trail(
                15,
                "Wildflower Wonderland Trail",
                7.2,
                167,
                "Immerse yourself in a wonderland of wildflowers on this picturesque trail. From vibrant meadows to lush forests, this trail is a paradise for nature lovers.",
                3
            )
        )

        // Trail 16
        database.TrailDAO().insert(
            Trail(
                16,
                "Alpine Adventure Expedition",
                12.5,
                432,
                "Embark on an alpine adventure through rugged terrain and towering peaks. This challenging expedition offers breathtaking views and a true test of endurance.",
                4
            )
        )

        // Trail 17
        database.TrailDAO().insert(
            Trail(
                17,
                "Secret Forest Sanctuary",
                10.8,
                500,
                "Discover a secret forest sanctuary hidden deep within the Tatras. With its ancient trees and mystical atmosphere, this trail offers a journey into the heart of nature.",
                3
            )
        )

        // Trail 18
        database.TrailDAO().insert(
            Trail(
                18,
                "Golden Sunrise Trail",
                6.0,
                167,
                "Witness the beauty of a golden sunrise on this early morning trail. As the first light of dawn illuminates the peaks, you'll be treated to a breathtaking spectacle.",
                2
            )
        )

        // Trail 19
        database.TrailDAO().insert(
            Trail(
                19,
                "Whispering Pines Path",
                4.5,
                120,
                "Follow the whispering pines through tranquil forests and rolling hills. This gentle path offers a peaceful retreat and a chance to reconnect with nature.",
                1
            )
        )

        // Trail 20
        database.TrailDAO().insert(
            Trail(
                20,
                "Echo Valley Expedition",
                14.0,
                567,
                "Embark on an expedition through the majestic Echo Valley. With its towering cliffs and echoing canyons, this trail offers an unforgettable adventure for intrepid hikers.",
                4
            )
        )

        // Trail 21
        database.TrailDAO().insert(
            Trail(
                21,
                "Dragon's Backbone Ridge",
                11.0,
                421,
                "Trek along the rugged spine of Dragon's Backbone Ridge, where jagged peaks and steep cliffs provide an exhilarating challenge. Perfect for seasoned hikers seeking adventure.",
                5
            )
        )

        // Trail 22
        database.TrailDAO().insert(
            Trail(
                22,
                "Sunset Peak Ascent",
                8.5,
                231,
                "Climb to the top of Sunset Peak and witness the breathtaking view as the sun dips below the horizon. This trail is a favorite among photographers and sunset chasers.",
                6
            )
        )

        // Trail 23
        database.TrailDAO().insert(
            Trail(
                23,
                "Thunderstorm Pass",
                12.2,
                360,
                "Navigate the challenging terrain of Thunderstorm Pass, where sudden weather changes can add an extra layer of excitement. Be prepared for a thrilling and unpredictable hike.",
                6
            )
        )

        // Trail 24
        database.TrailDAO().insert(
            Trail(
                24,
                "Eagle's Perch Trail",
                7.5,
                150,
                "Reach the lofty heights of Eagle's Perch and enjoy panoramic views of the surrounding valleys. This trail is known for its steep ascents and rewarding vistas.",
                5
            )
        )

        // Trail 25
        database.TrailDAO().insert(
            Trail(
                25,
                "Crimson Canyon Trek",
                9.0,
                342,
                "Journey through the vibrant rock formations of Crimson Canyon, where the red hues of the cliffs create a stunning backdrop. A moderately difficult trek with plenty of visual rewards.",
                5
            )
        )

        // Trail 26
        database.TrailDAO().insert(
            Trail(
                26,
                "Glacier Point Challenge",
                13.5,
                432,
                "Take on the Glacier Point Challenge, a demanding hike through icy landscapes and steep inclines. Perfect for hikers looking for a physically demanding and visually spectacular route.",
                7
            )
        )

        // Trail 27
        database.TrailDAO().insert(
            Trail(
                27,
                "Phantom Lake Loop",
                10.0,
                567,
                "Explore the eerie beauty of Phantom Lake, where misty waters and dense forests create an otherworldly atmosphere. This loop trail is both challenging and enchanting.",
                6
            )
        )

        // Trail 28
        database.TrailDAO().insert(
            Trail(
                28,
                "Mountain King’s Trail",
                8.8,
                129,
                "Ascend the Mountain King’s Trail, a path that winds through ancient forests and steep rock faces. This trail offers a royal challenge for those ready to take it on.",
                5
            )
        )

        // Trail 29
        database.TrailDAO().insert(
            Trail(
                29,
                "Serpent’s Tail Path",
                9.7,
                236,
                "Follow the twists and turns of Serpent’s Tail Path, a trail known for its winding routes and sudden drops. This hike is both thrilling and demanding, perfect for experienced adventurers.",
                6
            )
        )

        // Trail 30
        database.TrailDAO().insert(
            Trail(
                30,
                "Jagged Peak Traverse",
                14.3,
                420,
                "Traverse the rugged terrain of Jagged Peak, where sharp cliffs and narrow paths offer a true test of skill and endurance. This is one of the most challenging hikes in the region.",
                7
            )
        )

    }
    val achievementCount = database.AchievementDAO().getAchievementCount()
    if (achievementCount == 0) {
        database.AchievementDAO().insert(
            Achievement(
                id = 0,
                text = "Log into the app",
                achieved = true
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 1,
                text = "Made a photo",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 2,
                text = "Completed first trail",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 3,
                text = "Fast walker",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 4,
                text = "Slow walker",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 5,
                text = "Completed a trail 5 times",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 6,
                text = "Completed a trail 10 times",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 7,
                text = "Completed all easy trails",
                achieved = false
            )
        )
        database.AchievementDAO().insert(
            Achievement(
                id = 8,
                text = "Completed all hard trails",
                achieved = false
            )
        )
    }
    val variableCount = database.GlobalVariablesDAO().getVariablesCount()
    if (variableCount == 0) {
        database.GlobalVariablesDAO().insert(
            GlobalVariable(
                1,
                2.0
            )
        )
    }
}
