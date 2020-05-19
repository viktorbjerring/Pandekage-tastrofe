from dataclasses import dataclass
from datetime import datetime, timedelta

@dataclass
class Order:
    id: int
    timeDone: datetime
    status: int = 0
    timeCreated: datetime = datetime.utcnow()

    def __str__(self):
        done = self.timeDone.strftime("%Y-%m-%dT%H:%M:%S")
        created = self.timeCreated.strftime("%Y-%m-%dT%H:%M:%S")
        return f'Order[id: {self.id}, status: {self.status}, time_created: {created}, time_done: {done}]'

    def __repr__(self):
        return str(self)