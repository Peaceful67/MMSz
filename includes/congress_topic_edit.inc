<?php

/*
 *  $edit_topic => $selected_congress_item;
 * 
 *  -2 => Összes
 *  -1 => Általános
 *  0 < SQL-ből témakörönként
 */

if (isset($topic_edit_exit)) {
    unset($edit_topic);
    return;
}
$congressItemTopic = new CongressItemTopic();
$congressItemTopic->setSelectedItem($edit_topic);
if (isset($topic_add)) {
    $congressItemTopic->addTopicToItem();
}
if (isset($topic_delete)) {
    $congressItemTopic->deleteTopic($topic_delete);
}
if (isset($topic_save)) {
    $desc_var = 'topic_' . $topic_save;
    $congressItemTopic->setSelectedTopic($topic_save)->setTopicDescription($$desc_var);
}
$view->putPrevent();
$view->newKeret();
$view->putElement('<h3>Témakörök szerkesztése</h3>');

$topics = $congressItemTopic->getTopicsOfItem($edit_topic);
$view->submitAdd('topic_add', $value, 'Témakör hozzáadása');
$view->submitBack('topic_edit_exit', 'Visszatérés a témakörök szerkesztéséből');
$topics = $congressItemTopic->getTopicsOfItem($edit_topic);
foreach ($topics as $topic_id => $desc) {
    $view->newDiv('row');
    $var_name = 'topic_' . $topic_id;
    $$var_name = $desc;
    $view->inputText($var_name, '', 'Témakör');
    $view->submitSave('topic save', $topic_id, 'Témakörök mentése');
    if ($congressItemTopic->isTopicDeletable($topic_id)) {
        $view->submitDelete('topic_delete', $topic_id, 'Témakör törlése');
    }
    $view->endDiv();
}
$view->endKeret();
