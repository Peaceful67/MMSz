<?php

logger(-1, -1, LOGGER_CRONTAB, "Napi rutin");

$cron_tasks = [
    [
        'day' => 30,
        'name' => 'cleanup_roles',
    ],
];

$cronTab = new Crontab();

foreach ($cron_tasks as $task) {
    if($cronTab->isTaskToDo($task['day'], $task['name'])) {
        include_once FUNCTIONS.'cron_tasks.inc';
        $method = 'cron_'.$task['name'];
        if(function_exists($method)) {
            logger(-1, -1, LOGGER_CRONTAB, 'Végrehajtott feladat: '.$method);
            $method();
        } else {
            error_log('Cron daily: wrong method name:'.$method);
        }
    }
}
