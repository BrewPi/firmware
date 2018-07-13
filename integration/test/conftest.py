import asyncio
import os

import aiohttp
import pytest
from async_timeout import timeout


@pytest.yield_fixture(scope='session')
def event_loop():
    loop = asyncio.new_event_loop()
    yield loop
    loop.close()


@pytest.fixture(scope='session')
def host():
    addr = os.getenv('SPARK_ADDRESS', '127.0.0.1')
    port = os.getenv('SPARK_PORT', '5000')
    return f'http://{addr}:{port}'


@pytest.fixture(scope='module')
async def session(event_loop, host):

    # Helper function: make http calls to heartbeat / ping endpoints until they're ok
    async def wait_online(session, addr):
        while True:
            try:
                res = await session.get(addr)
                assert res.status < 400
                break
            except Exception:
                await asyncio.sleep(0.1)

    # Yield a client session after the Spark is online
    async with aiohttp.ClientSession(raise_for_status=True) as session:
        async with timeout(15):
            await wait_online(session, f'{host}/spark/_service/status')

        yield session
