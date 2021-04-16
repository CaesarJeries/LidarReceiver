#!/usr/bin/env python3

import udp
import time
import numpy as np
import logging
from collections import defaultdict



logging.basicConfig(level=logging.DEBUG, format='[%(asctime)s] %(message)s')

def generate_random_image(width=720, height=640):
    size = width * height
    random_image = np.random.randint(0, 255, size)

    return random_image


# create a client and randomly transmit 
# image frames at 30 fps
# server = udp.Server()
# server.listen()

class LidarDataContainer(object):

    def __init__(self,
                 height,
                 width,
                 fps,
                 min_chunk_size=1,
                 max_chunk_size=200):
        
        # todo: generate `fps` images, each image is split
        # to random chunks, each chunk size is in the range
        # [min_chunk_size, max_chunk_size]

        logging.debug('Creating a lidar data container:\nheight: {}, width: {}, fps: {}, min_chunk_size: {}, max_chunk_size:{}'
                     .format(height, width, fps, min_chunk_size, max_chunk_size))

        self.data = defaultdict(dict)
        self.base_frame_number = 0
        self.height = height
        self.width = width
        self.fps = fps
        self.min_chunk_size = min_chunk_size
        self.max_chunk_size = max_chunk_size
        self._populate()
    
    def _populate(self):
        for frame_number in range(self.base_frame_number, self.base_frame_number + self.fps):
            if frame_number in self.data:
                error_msg = 'Frame number {} is already in the container'.format(frame_number)
                logging.error(error_msg)
                raise RuntimeError(error_msg)
            
            logging.debug('Generating random image data')
            self.data[frame_number]['image'] = generate_random_image(self.width, self.height)
            self.data[frame_number]['ranges'] = []

            free_cells_number = self.height * self.width
            
            # generate random ranges for image access
            while free_cells_number > 0:
                logging.debug('Generating random ranges')
                # window size of current range
                random_window_size = np.random.randint(self.min_chunk_size, self.max_chunk_size, 1).item()
                free_cells_number -= random_window_size
                
                # get range end of the previously generated range
                range_start = self.data[frame_number]['ranges'][-1][1] if len(self.data[frame_number]['ranges']) > 0 else 0
                range_end = range_start + random_window_size
                max_size = self.width * self.height
                if range_end > max_size:
                    range_end = max_size
                    random_window_size = range_end - range_start
                logging.info('Creating a new range starting from: {}. Window size: {}. Range end: {}'
                             .format(range_start, random_window_size, range_end))

                self.data[frame_number]['ranges'].append((range_start, range_end))
            
            np.random.shuffle(self.data[frame_number]['ranges'])

    def get_next(self):
        # get the next chunk.
        # images in the time window [t - 15, t] must all
        # be transmitted before sending any image from the window
        # [t + 1, t + m], m > 0

        keys = self.data.keys()

        if len(keys) == 0:
            # all frames have been consumed
            # generate new frames
            self.base_frame_number += self.fps
            self._populate()

        # convert to a list since dict keys are not necessarily in the range [0, len(keys)]
        random_index = list(keys)[np.random.randint(0, len(keys), 1).item()]
        ranges = self.data[random_index]['ranges']
        if len(ranges) > 0:
            r = ranges.pop()
        else:
            # the frame has been entirely consumed
            logging.debug('Removing frame from container at index: {}'.format(random_index))
            del self.data[random_index]
        
        logging.debug('Retrieving range at index {}: {}'.format(r, random_index))
        
        (start_index, end_index) = r
        return {
            'data': self.data[random_index]['image'],
            'start_index': start_index,
            'end_index': end_index
        }

class RandomLidarDataGenerator(object):

    def __init__(self, height=50, width=20, fps=15):
        self.height = height
        self.width = width
        self.fps = fps
        self.client = udp.Client()

        self.data = LidarDataContainer(height, width, fps)

    def transmit(self):
        while not self.data.empty():
            next_data_chunk = self.data.get_next()
            logging.debug('Transmitting next chunk of data')
            self.client.send(next_data_chunk)

            # sleep interval in seconds
            interval = 1. / self.fps
            logging.debug('Sleeping for {} seconds'.format(interval))
            time.sleep(interval)


if __name__ == '__main__':
    container = LidarDataContainer(50, 20, 15)

    chunk = container.get_next()
    logging.info('Retrieved chunk: size: [{}, {})'.format(chunk['start_index'], chunk['end_index']))
