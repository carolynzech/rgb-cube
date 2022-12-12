# rgb-cube capstone
This is the branch for Rachel Fuller's (rfuller1) capstone portion of the project.
The difference in this branch is that the user can use a [website](https://rgb-led-app.herokuapp.com/) to specify where in the world they would like to get the weather from. In the main branch, this has to be hardcoded in the rgb-cube.ino file, which makes it difficult to change for the average user. The main changes to the project occur in fetch_weather, beginning in [line 110](https://github.com/carolynzech/rgb-cube/blob/capstone/fetch_weather.ino#L110) and in rgb-cube, beginning in [line 57](https://github.com/carolynzech/rgb-cube/blob/capstone/rgb-cube.ino#L57).

## How to run?
Clone the repo, plug in the RGB cube Arduino, and hit upload.

## How to run with tests?
Uncomment the test_all_capstone_tests(); line in rgb-cube setup(). This is line 53. They test functions that interact with the rgb-led-app API, primarily in their ability to parse responses correctly (i.e. the functions that I added for the capstone portion of the project).

## More information
Please see the [abstract](https://docs.google.com/document/d/1QkgTzXO2vBL-_1ITD4_wg6uiPxLPgOvYcsn2GpKttf4/edit) for an explanation of how this part of the project interacts with other components, like the [website code](https://github.com/rfuller1/rgb-cube-website).

