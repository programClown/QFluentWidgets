#ifndef TRIE_H
#define TRIE_H

#include <QString>

class Trie
{
public:
    Trie();

    void insert(QString key, int value);

    int get(QString key, int defaultValue = 0);

    Trie *searchPrefix(QString prefix);

    QMap<QString, int> items(QString prefix);

private:
    QString m_key        = "";
    int m_value          = 0;
    Trie *m_children[26] = { 0 };
    bool m_isEnd         = false;
};

#endif  // TRIE_H
