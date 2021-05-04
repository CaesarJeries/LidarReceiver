#!/usr/bin/env python3

import udp
import time
import numpy as np
from collections import defaultdict
from udp import Client, get_logger

logger = get_logger(__name__)


def generate_random_image(height=32, width=32):
    size = width * height
    random_image = np.random.randint(0, 256, size, dtype=np.uint8)
    return random_image


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

        logger.info('Creating a lidar data container:\nheight: {}, width: {}, fps: {}, min_chunk_size: {}, max_chunk_size:{}'
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
                logger.error(error_msg)
                raise RuntimeError(error_msg)
            
            logger.info('Generating random image data')
            self.data[frame_number]['image'] = generate_random_image(self.height, self.width)
            self.data[frame_number]['ranges'] = []

            free_cells_number = self.height * self.width
            
            # generate random ranges for image access
            while free_cells_number > 0:
                logger.info('Generating random ranges')
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
                logger.debug('Creating a new range starting from: {}. Window size: {}. Range end: {}'
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
            logger.debug('Removing frame from container at index: {}'.format(random_index))
            del self.data[random_index]
        
        logger.debug('Retrieving range at index {}: {}'.format(r, random_index))
        
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
        while True:
            next_data_chunk = self.data.get_next()
            logger.info('Transmitting next chunk of data')
            self.client.send(next_data_chunk)

            # sleep interval in seconds
            interval = 1. / self.fps
            logger.debug('Sleeping for {} seconds'.format(interval))
            time.sleep(interval)


class SequentialLidarDataGenerator(object):

    def __init__(self, client, frame_number=3, height=50, width=20, fps=15):
        self.height = height
        self.width = width
        self.fps = fps
        self.sleep_interval = 1. / fps
        self.frame_number = frame_number
        self.client = client
        self.data = [generate_random_image(height, width) for _ in range(frame_number)]

    def transmit(self):
        while True:
            try:
                frame_num_itr = 0
                data_element = self.data.pop().tobytes()
                header = frame_num_itr.to_bytes(4, 'big')
                start_idx = 0
                end_index = 32**2 - 1
                header += start_idx.to_bytes(4, 'big')
                header += end_index.to_bytes(4, 'big')

                logger.debug('Sending header: {}'.format(header))
                client.send(header)

                logger.debug('Sending data')
                client.send(data_element)
                time.sleep(self.sleep_interval)
            except IndexError:
                # all data was consumed
                break
                    


if __name__ == '__main__':
    client = Client()
    data_gen = SequentialLidarDataGenerator(client, fps=1)

    data_gen.transmit()
