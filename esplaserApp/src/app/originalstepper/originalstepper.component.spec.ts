import { ComponentFixture, TestBed } from '@angular/core/testing';

import { OriginalstepperComponent } from './originalstepper.component';

describe('OriginalstepperComponent', () => {
  let component: OriginalstepperComponent;
  let fixture: ComponentFixture<OriginalstepperComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ OriginalstepperComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(OriginalstepperComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
