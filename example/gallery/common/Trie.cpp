#include "Trie.h"

#include <QQueue>
#include <QMap>

Trie::Trie() { }

void Trie::insert(QString key, int value)
{
    key = key.toLower();

    Trie *node = this;

    for (int j = 0; j < key.length(); j++) {
        int i = key[j].unicode() - 'a';
        if (i < 0 || i >= 26)
            return;

        if (!node->m_children[i]) {
            node->m_children[i] = new Trie();
        }

        node = node->m_children[i];
    }

    node->m_isEnd = true;
    node->m_key   = key;
    node->m_value = value;
}

int Trie::get(QString key, int defaultValue)
{
    Trie *node = searchPrefix(key);
    if (!(node && node->m_isEnd)) {
        return defaultValue;
    }
    return node->m_value;
}

Trie *Trie::searchPrefix(QString prefix)
{
    prefix     = prefix.toLower();
    Trie *node = this;

    for (int j = 0; j < prefix.length(); j++) {
        int i = prefix[j].unicode() - 'a';

        if (!(i >= 0 && i < 26 && node->m_children[i])) {
            return nullptr;
        }

        node = node->m_children[i];
    }

    return node;
}

QMap<QString, int> Trie::items(QString prefix)
{
    QMap<QString, int> result;

    Trie *node = searchPrefix(prefix);
    if (!node) {
        return result;
    }

    QQueue<Trie *> q;
    q.append(node);

    while (!q.empty()) {
        node = q.takeFirst();
        if (node->m_isEnd) {
            result.insert(node->m_key, node->m_value);
        }

        for (Trie *c : node->m_children) {
            if (c) {
                q.append(c);
            }
        }
    }
}
