import qbattlelib
import json

def init():
	qbattlelib.init()

def start():
	qbattlelib.start()

def update(arr1,arr2,arr3,arr4,arr5):
	return json.loads(qbattlelib.update(arr1,arr2,arr3,arr4,arr5))

