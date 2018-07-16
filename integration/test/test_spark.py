"""
Tests the brewblox-devcon-spark service
"""

import pytest
from aiohttp.client_exceptions import ClientResponseError


@pytest.fixture
def sensey():
    return {
        'id': 'sensey',
        'type': 'OneWireTempSensor',
        'data': {
            'settings': {
                'address': 'FF',
                'offset[delta_degF]': 20
            }
        }
    }


@pytest.mark.asyncio
async def test_profiles(session, host):
    # create
    url = host + '/spark/profiles'
    retv = session.post(url)
    retd = await retv.json()
    assert 'profile_id' in retd

    # activate
    id = retd['profile_id']
    await session.post(f'{url}/{id}')

    # list
    retv = session.get(url)
    retd = await retv.json()
    assert 'profiles' in retd
    assert retd['profile_id'] == id

    # create + delete
    retv = session.post(url)
    retd = await retv.json()
    delid = retd['profile_id']
    await session.delete(f'{url}/{delid}')


@pytest.mark.asyncio
async def test_create_objects(session, host, sensey):
    create_url = host + '/spark/objects'
    retv = await session.post(create_url, json=sensey)
    retd = await retv.json()
    print(retd)

    assert retd['id'] == sensey['id']
    assert retd['data']['settings']['offset[delta_degC]']

    sensey['id'] = 'sensex'
    await session.post(create_url, json=sensey)

    with pytest.raises(ClientResponseError):
        del sensey['id']
        await session.post(create_url, json=sensey)


@pytest.mark.asyncio
async def test_read_objects(session, host, sensey):
    retv = await session.get(host + '/spark/objects/sensey')
    retd = await retv.json()
    print(retd)

    assert retd['id'] == 'sensey'
    assert retd['data']['settings']['offset[delta_degC]']

    await session.get(host + '/spark/objects/sensex')


@pytest.mark.asyncio
async def test_read_all(session, host):
    retv = await session.get(host + '/spark/objects')
    retd = await retv.json()

    assert len(retd) == 2
    assert retd[0]['id'] == 'sensey'
    assert retd[1]['id'] == 'sensex'

    assert retd[0]['type'] == 'OneWireTempSensor'


@pytest.mark.asyncio
async def test_write_object(session, host, sensey):
    url = host + '/spark/objects/sensey'
    del sensey['id']
    sensey['data']['settings']['offset[delta_degF]'] = 30

    await session.put(url, json=sensey)
    retv = await session.get(url)
    retd = await retv.json()

    assert retd['data']['settings']['offset[delta_degC]'] == pytest.approx(16.7, 0.1)


@pytest.mark.asyncio
async def test_delete_object(session, host, sensey):
    url = host + '/spark/objects'
    sensey['id'] = 'sensey_del'

    await session.post(url, json=sensey)
    await session.get(url + '/sensey_del')
    await session.delete(url + '/sensey_del')

    with pytest.raises(ClientResponseError):
        await session.get(url + '/sensey_del')


@pytest.mark.asyncio
async def test_read_system(session, host):
    retv = await session.get(host + '/spark/system/onewirebus')
    retd = await retv.json()
    print(retd)

    assert retd['data']['address']


@pytest.mark.asyncio
async def test_write_system(session, host):
    retv = await session.get(host + '/spark/system/onewirebus')
    retd = await retv.json()
    print(retd)

    retd['data']['address'].append('cc')
    await session.put(host + '/spark/system/onewirebus', json=retd)

    retv = await session.get(host + '/spark/system/onewirebus')
    retd = await retv.json()
    print(retd)

    assert 'cc' in retd['data']['address']
