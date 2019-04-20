#! /usr/bin/python3

import logging, logging.handlers

rootLogger = logging.getLogger('')
rootLogger.setLevel(logging.DEBUG)
socketHandler = logging.handlers.SocketHandler('localhost',
                    logging.handlers.DEFAULT_TCP_LOGGING_PORT)

rootLogger.addHandler(socketHandler)

logging.info('Root logging info test.')

logger1 = logging.getLogger('demo.part1')
logger2 = logging.getLogger('demo.part2')

for i in range(10000):
  logger1.debug('Part 1 has a lazy debugging message.')
  logger1.info('Part 1 is doing some info now.')
  logger2.warning('Number 2 has a warning 4 you.')
  logger2.error('Number 2 emits error')

