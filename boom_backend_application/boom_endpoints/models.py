from django.db import models

from datetime import datetime
# Create your models here.


class ActivityLog(models.Model):
    id = models.AutoField(primary_key=True)
    last_entry_time = models.DateTimeField(auto_now=True)

    class Meta:
        db_table = 'activity_logs'


class BoomSig(models.Model):
    id = models.AutoField(primary_key=True)
    entryboom = models.CharField(max_length=1, default='N')
    entrysynctime = models.DateTimeField(auto_now_add=True)
    exitboom = models.CharField(max_length=1, default='N')
    exitsynctime = models.DateTimeField(auto_now_add=True)

    class Meta:
        db_table = 'boomsig'
