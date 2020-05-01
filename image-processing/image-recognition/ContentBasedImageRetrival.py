import cv2
import numpy


# create histograms database and takes input image from user
def contend_based_image_retrieval():
    number_of_hist = create_images_histograms()
    #number_of_hist = 70
    image_name = input("> Write image name which you want to find : ")
    while image_name is not None:
        index_of_images = find_similar_image(number_of_hist, image_name, True)  # find similar based on RGB
        if index_of_images != -1:
            print("Similar image based on colour:")
            for i in index_of_images:
                print("     data_image/image_00" + str(i))
                # show_image(index_of_images)

            index_of_images = find_similar_image(number_of_hist, image_name, False)  # find similar based on LBP
            print("Similar image based on LBP:")
            for i in index_of_images:
                print("     data_image/image_00" + str(i))
            # show_image(index_of_images)

        image_name = input("> Write image name which you want to find : ")


# create histograms of test images and save all histograms in the 'histograms' file
def create_images_histograms():
    flag = 0
    i = 0
    while flag == 0:
        image_name = 'data_image/image_00' + str(i) + '.jpg'
        image = cv2.imread(image_name)  # image is read with OpenCV library
        if image is None:
            print('> All image`s histograms have been writen in a file\n')
            flag = 1
        else:
            print('> Histograms of  the image_00' + str(i) + ' are creating... ')
            file_name = 'histograms/hist_00' + str(i) + '.npy'
            numpy.save(file_name, get_histogram(image))
            i += 1

    return i


# create histograms of searching image and find 5 image which is similar to it.
# is_rgb : true if we compare images based on RGB, false if we compare images based on LBP
def find_similar_image(number_of_hist, image_name, is_rgb):
    image = cv2.imread(image_name)
    if image is None:
        print('Image not found! <Usage : test_image/image_0016.jpg >')
        return -1

    image_histogram = get_histogram(image)
    distance_list = numpy.zeros(number_of_hist, dtype=numpy.double)
    for i in range(0, number_of_hist):
        file_name = 'histograms/hist_00' + str(i) + '.npy'
        saved_histogram = numpy.load(file_name)
        distance_list[i] = distance(saved_histogram, image_histogram, is_rgb)  # store the distance

    similar_image_index_list = get_index(distance_list, number_of_hist)
    return similar_image_index_list


# take the distances, find minimum 5 distance and return their index(which is index of image).
def get_index(distance_list, size):
    min_index_list = []
    for i in range(0, 5):
        min_index = 0
        for j in range(i + 1, size):
            if distance_list[j] < distance_list[min_index]:
                min_index = j

        min_index_list.append(min_index)
        distance_list[min_index] = 100  # When the minimum distance is found, make it's value 100
    return min_index_list


def lbp(gray_image, rows, cols):
    lbp_image = numpy.zeros((rows, cols), dtype=numpy.uint8)
    for i in range(1, rows - 1):
        for j in range(1, cols - 1):
            current_pixel = gray_image[i][j]
            new_pixel = 0
            if gray_image[i - 1][j - 1] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i - 1][j] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i - 1][j + 1] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i][j + 1] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i + 1][j + 1] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i + 1][j] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i + 1][j - 1] > current_pixel:
                new_pixel += 1
            new_pixel *= 2
            if gray_image[i][j - 1] > current_pixel:
                new_pixel += 1

            lbp_image[i][j] = new_pixel
    return lbp_image


def normalize(hist, number_of_pixels):
    for i in range(0, 256):
        hist[0][i] = (hist[0][i] / number_of_pixels)
        hist[1][i] = (hist[1][i] / number_of_pixels)
        hist[2][i] = (hist[2][i] / number_of_pixels)
        hist[3][i] = (hist[3][i] / number_of_pixels)

    return hist


def get_histogram(image):
    rows = image.shape[0]  # assign rows
    cols = image.shape[1]  # assign cols

    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    lbp_image = lbp(gray_image, rows, cols)  # calculate lbp of the mage

    hist = numpy.zeros((4, 256), dtype=numpy.double)  # define 4 Histogram and arrange them as 0(int)
    for i in range(0, rows):
        for j in range(0, cols):
            blue_index = image[i, j, 0]  # look at the current pixel's blue value and increase
            green_index = image[i, j, 1]  # look at the current pixel's green value and increase
            red_index = image[i, j, 2]  # look at the current pixel's red value and increase
            lbp_index = lbp_image[i, j]

            hist[0][blue_index] += 1
            hist[1][green_index] += 1
            hist[2][red_index] += 1
            hist[3][lbp_index] += 1

    normalized_hist = normalize(hist, rows * cols)
    return normalized_hist


def distance(hist_1, hist_2, is_rgb):
    if is_rgb:
        dist = 0
        for i in range(0, 256):
            dist += abs(hist_1[0][i] - hist_2[0][i])
            dist += abs(hist_1[1][i] - hist_2[1][i])
            dist += abs(hist_1[2][i] - hist_2[2][i])
    else:
        dist = 0
        for i in range(0, 256):
            dist += abs(hist_1[3][i] - hist_2[3][i])

    return dist


def show_image(index_of_image):
    image_name = 'data_image/image_00' + str(index_of_image) + '.jpg'
    image = cv2.imread(image_name)  # image is read with OpenCV library
    if image is None:
        print('> Image not found')
        return -2
    cv2.imshow('Result', image)
    cv2.waitKey(0)


contend_based_image_retrieval()
