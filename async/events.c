#include "events.h"

#define PutEvent(map, key, value) map->put(map, (let)key, (let)value)
#define GetEvent(map, key) (Array)map->get(map, (let)key)
#define RemoveEvent(map, key) map->remove(map, (let)key)
#define ExisteEvent(map, key) map->exists(map, (let)key)

Emitter createEmitter() {
	Emitter emitter = newEmitter();
	if (emitter == NULL) {
		return NULL;
	}
	emitter->hashMap = createHashMap(NULL, NULL);
	emitter->on = on;
	emitter->emit = emit;
	emitter->once = once;
	emitter->prependListener = prependListener;
	emitter->prependOnceListener = prependOnceListener;
	emitter->removeListener = removeListener;
	emitter->getEventListener = getEventListener;
	return emitter;
}

void freeEmitter(Emitter * emitter) {
	(*emitter)->hashMap->clear((*emitter)->hashMap);
	free((*emitter)->hashMap);
	free(*emitter);
	*emitter = NULL;
}

void on(Emitter emitter, string eventName, Listener listener, let args) {
	HashMap hashMap = emitter->hashMap;
	Event event = newEvent();
	if (event == NULL) {
		return NULL;
	}
	event->listener = listener;
	event->type = MoreEvent;
	event->eventName = newString(eventName);
	event->args = args;
	Array eventQueue = NULL;
	if (ExisteEvent(hashMap, event->eventName)) {
		eventQueue = GetEvent(hashMap, event->eventName);
	}
	else {
		eventQueue = createArray(0);
		PutEvent(hashMap, event->eventName, eventQueue);
	}
	eventQueue->push(eventQueue, event);
}

void once(Emitter emitter, string eventName, Listener listener, let args) {
	HashMap hashMap = emitter->hashMap;
	Event event = newEvent();
	if (event == NULL) {
		return;
	}
	event->listener = listener;
	event->type = OnceEvent;
	event->eventName = newString(eventName);
	event->args = args;
	Array eventQueue = NULL;
	if (ExisteEvent(hashMap, event->eventName)) {
		eventQueue = GetEvent(hashMap, event->eventName);
	}
	else {
		eventQueue = createArray(0);
        string x = newString(event->eventName);
		PutEvent(hashMap, x, eventQueue);
	}
	eventQueue->push(eventQueue, event);
	assert(eventQueue->length > 0);
}

void emit(Emitter emitter, string eventName) {
	HashMap hashMap = emitter->hashMap;
	if (ExisteEvent(hashMap, eventName)) {
		eventName = newString(eventName);
		Array eventQueue = GetEvent(hashMap, eventName);
		ArrayIterator iterator = createArrayIterator(eventQueue);
		while (hasNextArrayIterator(iterator)) {
			iterator = nextArrayIterator(iterator);
			ListNode node = iterator->node;
			if (node == NULL) {
				break;
			}
			Event event = (Event)node->item;
			event->listener(event, event->args);
			if (event->type == OnceEvent) {

				// �¼����г��ȼ���1
				eventQueue->length--;

				// ��������ָ�򷵻���һ��
				iterator->node = iterator->node->last;

				// �޸�����ڵ�
				if (node->last == NULL) {
					eventQueue->head = node->next;
					if (eventQueue->head != NULL) {
						eventQueue->head->last = NULL;
					}
				}
				else if (node->next == NULL) {
					eventQueue->tail = node->last;
					if (eventQueue->tail != NULL) {
						eventQueue->tail->next = NULL;
					}
				}
				else {
					node->last->next = node->next;
				}

				free(event->eventName);

				// �ͷ�event�ڵ�
				free(event);

				// �ͷŽڵ�
				free(node);
			}
		}
		if (eventQueue->length == 0) {
			// ����¼�����Ϊ�գ����Ƴ��¼�����
            free(RemoveEvent(hashMap, eventName));
			freeArray(&eventQueue);
		}
		freeArrayIterator(&iterator);
		free(eventName);
	}
}

void prependListener(Emitter emitter, string eventName, Listener listener, let args) {
	HashMap hashMap = emitter->hashMap;
	Event event = newEvent();
	if (event == NULL) {
		return NULL;
	}
	event->listener = listener;
	event->type = MoreEvent;
	event->eventName = newString(eventName);
	event->args = args;
	Array eventQueue = NULL;
	if (ExisteEvent(hashMap, eventName)) {
		eventQueue = GetEvent(hashMap, eventName);
	}
	else {
		eventQueue = createArray(0);
		PutEvent(hashMap, eventName, eventQueue);
	}
	eventQueue->unshift(eventQueue, event);
}

void prependOnceListener(Emitter emitter, string eventName, Listener listener, let args) {
	HashMap hashMap = emitter->hashMap;
	Event event = newEvent();
	if (event == NULL) {
		return;
	}
	event->listener = listener;
	event->type = OnceEvent;
	event->eventName = newString(eventName);
	event->args = args;
	Array eventQueue = NULL;
	if (ExisteEvent(hashMap, eventName)) {
		eventQueue = GetEvent(hashMap, eventName);
	}
	else {
		eventQueue = createArray(0);
		PutEvent(hashMap, eventName, eventQueue);
	}
	eventQueue->unshift(eventQueue, event);
}

void removeListener(Emitter emitter, string eventName) {
	HashMap hashMap = emitter->hashMap;
	if (ExisteEvent(hashMap, eventName)) {
		Array array = GetEvent(hashMap, eventName);
		freeArray(&array);
		RemoveEvent(hashMap, eventName);
	}
}

Array getEventListener(Emitter emitter, string eventName) {
	HashMap hashMap = emitter->hashMap;
	if (ExisteEvent(hashMap, eventName)) {
		return  GetEvent(hashMap, eventName);
	}
	else {
		return NULL;
	}
}

void dispalyEmitter(Emitter emitter) {
	HashMap hashMap = emitter->hashMap;
	HashMapIterator iterator = createHashMapIterator(hashMap);
	while (hasNextHashMapIterator(iterator)) {
		iterator = nextHashMapIterator(iterator);
		log("id: %s\n", (string)iterator->entry->key);
		log("hashCode: %d\n", iterator->hashCode);
	}
	freeHashMapIterator(&iterator);
	log("\n\n");
}
