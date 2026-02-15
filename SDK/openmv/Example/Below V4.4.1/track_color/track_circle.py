# Untitled - By: Weact - 7 8 2020
red_threshold = (53, 31, 44, 82, 18, 78)
area = (0, 0, 320, 240)
import sensor, image, time, lcd

clock = time.clock()

sensor.reset()
if sensor.get_id() == sensor.OV7725:
    sensor.set_hmirror(True)
    sensor.set_vflip(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(20)
sensor.set_auto_whitebal(False)
sensor.set_auto_gain(False)
# Close the white balance.White Balance is turned on by default. In color recognition, white balance needs to be turned off
clock = time.clock()

# search max blob
def search_max(blobs):
    max_blob = 0
    max_size = 0
    for blob in blobs:
        blob_area = blob[2] * blob[3]
        if blob_area > max_size:
            max_blob = blob
            max_size = blob_area
    return max_blob


# Find the largest circle position in the frame
def search_max_circle(ball_threshold, area):
    clock.tick()  # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot()  # Take a picture and return the image.
    blobs = img.find_blobs([ball_threshold], roi=area)
    print(blobs)
    if blobs:
        max_blob = search_max(blobs)
        x = max_blob[0] - 3
        y = max_blob[1] - 3
        w = max_blob[2] + 6
        h = max_blob[3] + 6
        outside_rect = (x, y, w, h)
        print(max_blob)
        print("fps:", clock.fps())
        img.draw_rectangle(max_blob[0:4])  # rect
        img.draw_rectangle(outside_rect[0:4])  # rect
        img.draw_cross(max_blob[5], max_blob[6]) # cx,cy
        return max_blob


# Follow the largest circle
def track_max_circle(red_threshold, area1):
    global area
    track = search_max_circle(red_threshold, area1)
    if track:
        x = track[0] - 5 if track[0] - 5 > 0 else 0
        y = track[1] - 5 if track[1] - 5 > 0 else 0
        w = track[2] + 10
        h = track[3] + 10
        area = (x, y, w, h)
        print('area=', area)
    else:
        print('out of range')
        area = (0, 0, 320, 240)



while(True):
    track_max_circle(red_threshold, area)
