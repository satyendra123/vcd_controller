# Generated by Django 3.2.12 on 2025-01-28 17:58

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        ('dashboard', '0003_delete_anpr'),
    ]

    operations = [
        migrations.CreateModel(
            name='CarLog',
            fields=[
                ('id', models.AutoField(primary_key=True, serialize=False)),
                ('gate', models.CharField(max_length=64)),
                ('action', models.CharField(max_length=64)),
                ('created_at', models.DateTimeField(auto_now_add=True)),
                ('updated_at', models.DateTimeField(auto_now=True)),
            ],
        ),
    ]
