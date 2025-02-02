from django.db.models import fields
from rest_framework import serializers
from .models import BoomSig, ActivityLog
 
class BoomSerializer(serializers.ModelSerializer):
    class Meta:
        model = BoomSig
        fields = '__all__'

class ActivityLogSerializer(serializers.ModelSerializer):
    class Meta:
        model = ActivityLog
        fields = '__all__'
